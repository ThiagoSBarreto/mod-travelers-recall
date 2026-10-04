#include "TravelersRecallCommandScript.h"
#include "TravelersRecallService.h"

namespace
{
    template <typename S>
    bool IsClientlessSessionImpl(S const* session)
    {
        if constexpr (requires { session->IsHeadless(); })
            return session->IsHeadless();
        else
            return session->IsBot();
    }

    bool IsClientlessSession(WorldSession const* session)
    {
        return session && IsClientlessSessionImpl(session);
    }

    std::string BuildCooldownRemainingText(uint32 remainingSeconds)
    {
        uint32 hours = remainingSeconds / 3600;
        uint32 minutes = (remainingSeconds % 3600) / 60;
        uint32 seconds = remainingSeconds % 60;

        std::vector<std::string> parts;

        if (hours > 0)
        {
            parts.push_back(Acore::StringFormat("{} hour{}", hours, hours == 1 ? "" : "s"));
        }

        if (minutes > 0)
        {
            parts.push_back(Acore::StringFormat("{} min{}", minutes, minutes == 1 ? "" : "s"));
        }

        if (seconds > 0 || parts.empty())
        {
            parts.push_back(Acore::StringFormat("{} sec", seconds));
        }

        std::string message;
        for (size_t i = 0; i < parts.size(); ++i)
        {
            if (i > 0)
            {
                message += " ";
            }
            message += parts[i];
        }

        return message;
    }
}

TravelersRecallCommandScript::TravelersRecallCommandScript()
    : CommandScript("TravelersRecallCommandScript")
{
}

Acore::ChatCommands::ChatCommandTable TravelersRecallCommandScript::GetCommands() const
{
    static Acore::ChatCommands::ChatCommandTable teleportTable =
        {
            { "list", HandleListCommand, SEC_PLAYER, Acore::ChatCommands::Console::No },
            { "learn", HandleLearnCommand, SEC_GAMEMASTER, Acore::ChatCommands::Console::No },
            { "learn all", HandleLearnAllCommand, SEC_GAMEMASTER, Acore::ChatCommands::Console::No },
            { "teleport", HandleTeleportCommand, SEC_PLAYER, Acore::ChatCommands::Console::No },
            { "remove", HandleRemoveCommand, SEC_GAMEMASTER, Acore::ChatCommands::Console::No },
            { "remove all", HandleRemoveAllCommand, SEC_GAMEMASTER, Acore::ChatCommands::Console::No }
        };

    static Acore::ChatCommands::ChatCommandTable commandTable =
        {
            {"tr", teleportTable}
        };

    return commandTable;
}

bool TravelersRecallCommandScript::SendListForPlayer(Player* player)
{
    QueryResult unlocks = CharacterDatabase.Query(
        "SELECT location_id FROM custom_travelers_recall_unlocks WHERE guid = {}",
        player->GetGUID().GetCounter());

    if (!unlocks)
    {
        return true;
    }

    do
    {
        Field* unlockFields = unlocks->Fetch();

        uint32 locationId = unlockFields[0].Get<uint32>();

        QueryResult location = WorldDatabase.Query(
            "SELECT name, icon FROM custom_travelers_recall_locations WHERE id = {}",
            locationId);

        if (!location)
        {
            continue;
        }

        Field* locationFields = location->Fetch();

        std::string locationName = locationFields[0].Get<std::string>();
        std::string icon = locationFields[1].Get<std::string>();

        std::string message = Acore::StringFormat(
            "TR_LIST:{}:{}:{}",
            locationId,
            locationName,
            icon);

        ChatHandler(player->GetSession()).SendSysMessage(message.c_str());

    } while (unlocks->NextRow());

    return true;
}

bool TravelersRecallCommandScript::HandleListCommand(ChatHandler* handler, char const* /*args*/)
{
    Player* player = handler->GetPlayer();
    return SendListForPlayer(player);
}

bool TravelersRecallCommandScript::HandleTeleportCommand(ChatHandler* handler, char const* args)
{
    Player* player = handler->GetPlayer();
    uint32 locationId = atoi(args);

    if (!TravelersRecall::CanPlayerTeleport(player, handler, locationId))
    {
        return true;
    }

    QueryResult location = WorldDatabase.Query(
        "SELECT map_id, position_x, position_y, position_z, orientation, name, cooldown FROM custom_travelers_recall_locations WHERE id = {}",
        locationId);

    if (!location)
    {
        handler->SendSysMessage("Traveler's Recall: location not found.");
        return false;
    }

    Field* fields = location->Fetch();

    uint32 mapId = fields[0].Get<uint32>();
    float x = fields[1].Get<float>();
    float y = fields[2].Get<float>();
    float z = fields[3].Get<float>();
    float o = fields[4].Get<float>();
    std::string locationName = fields[5].Get<std::string>();
    uint32 cooldown = fields[6].Get<uint32>();

    if (cooldown == 0 && sConfigMgr->GetOption<bool>("TravelersRecall.UseDefaultCooldown", false))
    {
        cooldown = sConfigMgr->GetOption<uint32>("TravelersRecall.DefaultCooldown", 0);
    }

    if (cooldown > 0)
    {
        QueryResult cooldownResult = CharacterDatabase.Query(
            "SELECT cooldown_end FROM custom_travelers_recall_unlocks WHERE guid = {} AND location_id = {}",
            player->GetGUID().GetCounter(),
            locationId);

        if (cooldownResult)
        {
            uint32 cooldownEnd = cooldownResult->Fetch()[0].Get<uint32>();
            uint32 now = duration_cast<Seconds>(GameTime::GetSystemTime().time_since_epoch()).count();

            if (cooldownEnd > now)
            {
                uint32 remaining = cooldownEnd - now;
                std::string remainingText = BuildCooldownRemainingText(remaining);

                handler->PSendSysMessage(
                    "Traveler's Recall: cooldown remaining for {}: {}.",
                    locationName,
                    remainingText);

                return true;
            }
        }
    }

    if (!player->TeleportTo(mapId, x, y, z, o))
    {
        handler->SendSysMessage("Traveler's Recall: teleportation failed.");
        return false;
    }
    else
    {
        if (Group* group = player->GetGroup())
        {
            for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
            {
                Player* partyMember = itr->GetSource();

                if (!partyMember)
                {
                    continue;
                }

                if (!partyMember->GetSession())
                {
                    continue;
                }

                if (!partyMember->IsInWorld())
                {
                    continue;
                }

                if (IsClientlessSession(partyMember->GetSession()))
                {
                    if (!partyMember->TeleportTo(mapId, x, y, z, o))
                    {
                        handler->SendSysMessage("Traveler's Recall: party bot teleportation failed.");
                    }
                }
            }
        }
    }

    if (cooldown > 0)
    {
        uint32 cooldownEnd = duration_cast<Seconds>(GameTime::GetSystemTime().time_since_epoch()).count() + cooldown;

        CharacterDatabase.Execute(
            "UPDATE custom_travelers_recall_unlocks SET cooldown_end = {} WHERE guid = {} AND location_id = {}",
            cooldownEnd,
            player->GetGUID().GetCounter(),
            locationId);
    }

    handler->PSendSysMessage(
        "Traveler's Recall: teleported to {}",
        locationName);

    return true;
}

bool TravelersRecallCommandScript::HandleLearnCommand(ChatHandler* handler, char const* args)
{
    Player* target = handler->getSelectedPlayerOrSelf();

    if (!target)
    {
        return false;
    }

    if (!args || !*args)
    {
        handler->SendSysMessage("Traveler's Recall: location id required.");
        return true;
    }

    uint32 areaId = atoi(args);

    if (areaId == 0)
    {
        handler->SendSysMessage("Traveler's Recall: invalid area id.");
        return true;
    }

    uint32 faction = target->GetTeamId() == TEAM_ALLIANCE ? 1 : 2;

    QueryResult result = WorldDatabase.Query(
        "SELECT id, name, icon FROM custom_travelers_recall_locations WHERE faction IN (0, {}) AND area_id = {}",
        faction,
        areaId);

    if (!result)
    {
        handler->SendSysMessage("Traveler's Recall: location not found.");
        return true;
    }

    Field* locFields = result->Fetch();

    uint32 locationId = locFields[0].Get<uint32>();
    std::string locationName = locFields[1].Get<std::string>();
    std::string icon = locFields[2].Get<std::string>();

    CharacterDatabase.Execute(
        "INSERT IGNORE INTO custom_travelers_recall_unlocks (guid, location_id, unlocked_at, cooldown_end) VALUES ({}, {}, NOW(), 0)",
        target->GetGUID().GetCounter(),
        locationId);

    handler->SendSysMessage("Traveler's Recall: location unlocked.");

    TravelersRecall::SendUnlockMessage(target, locationId, locationName, icon);

    return true;
}

bool TravelersRecallCommandScript::HandleLearnAllCommand(ChatHandler* handler, char const* args)
{
    Player* target = handler->getSelectedPlayerOrSelf();

    if (!target)
    {
        return false;
    }

    uint32 faction = target->GetTeamId() == TEAM_ALLIANCE ? 1 : 2;

    QueryResult result = WorldDatabase.Query(
        "SELECT id, name, icon FROM custom_travelers_recall_locations WHERE faction IN (0, {})",
        faction);

    if (!result)
    {
        handler->SendSysMessage("Traveler's Recall: location list not found.");
        return false;
    }

    uint32 guid = target->GetGUID().GetCounter();

    do
    {
        Field* fields = result->Fetch();

        uint32 locationId = fields[0].Get<uint32>();
        std::string locationName = fields[1].Get<std::string>();
        std::string icon = fields[2].Get<std::string>();

        CharacterDatabase.Execute(
            "INSERT IGNORE INTO custom_travelers_recall_unlocks (guid, location_id, unlocked_at, cooldown_end) VALUES ({}, {}, NOW(), 0)",
            guid,
            locationId);

        TravelersRecall::SendUnlockMessage(target, locationId, locationName, icon);

    } while (result->NextRow());

    handler->SendSysMessage("Traveler's Recall: all locations unlocked.");

    return true;
}

bool TravelersRecallCommandScript::HandleRemoveCommand(ChatHandler* handler, char const* args)
{
    Player* target = handler->getSelectedPlayerOrSelf();

    if (!target)
    {
        return false;
    }

    if (!args || !*args)
    {
        handler->SendSysMessage("Traveler's Recall: location id required");
        return true;
    }

    uint32 areaId = atoi(args);

    if (areaId == 0)
    {
        handler->SendSysMessage("Traveler's Recall: invalid area id.");
        return true;
    }

    uint32 faction = target->GetTeamId() == TEAM_ALLIANCE ? 1 : 2;

    QueryResult result = WorldDatabase.Query(
        "SELECT id FROM custom_travelers_recall_locations WHERE faction IN (0, {}) AND area_id = {}",
        faction,
        areaId);

    if (!result)
    {
        handler->SendSysMessage("Traveler's Recall: location not found.");
        return true;
    }

    uint32 locationId = result->Fetch()[0].Get<uint32>();

    CharacterDatabase.Execute(
        "DELETE FROM custom_travelers_recall_unlocks WHERE guid = {} AND location_id = {}",
        target->GetGUID().GetCounter(),
        locationId);

    handler->SendSysMessage("Traveler's Recall: location removed.");

    TravelersRecall::SendRemoveMessage(target, locationId);

    return true;
}

bool TravelersRecallCommandScript::HandleRemoveAllCommand(ChatHandler* handler, char const* args)
{
    Player* target = handler->getSelectedPlayerOrSelf();

    if (!target)
    {
        return false;
    }

    uint32 guid = target->GetGUID().GetCounter();

    QueryResult result = CharacterDatabase.Query(
        "SELECT location_id FROM custom_travelers_recall_unlocks WHERE guid = {}",
        guid);

    if (!result)
    {
        handler->SendSysMessage("Traveler's Recall: no locations to remove.");
        return true;
    }

    std::vector<uint32> ids;

    do
    {
        Field* fields = result->Fetch();
        ids.push_back(fields[0].Get<uint32>());
    } while (result->NextRow());

    CharacterDatabase.Execute(
        "DELETE FROM custom_travelers_recall_unlocks WHERE guid = {}",
        guid);

    for (uint32 locationId : ids)
    {
        TravelersRecall::SendRemoveMessage(target, locationId);
    }

    handler->SendSysMessage("Traveler's Recall: all locations removed.");

    return true;
}

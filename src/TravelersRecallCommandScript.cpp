#include "TravelersRecallCommandScript.h"
#include "TravelersRecallService.h"

namespace
{
    template <typename Session>
    bool IsBotSessionImpl(Session const *session)
    {
        if constexpr (requires { session->IsBot(); })
            return session->IsBot();
        else
            return false;
    }

    bool IsBotSession(WorldSession const *session)
    {
        return session && IsBotSessionImpl(session);
    }

    template <typename Session>
    bool IsHeadlessClientSessionImpl(Session const *session)
    {
        if constexpr (requires { session->IsHeadlessClient(); })
            return session->IsHeadlessClient();
        else
            return false;
    }

    bool IsHeadlessClientSession(WorldSession const *session)
    {
        return session && IsHeadlessClientSessionImpl(session);
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
            {"list", HandleListCommand, SEC_PLAYER, Acore::ChatCommands::Console::No},
            {"learn", HandleLearnCommand, SEC_GAMEMASTER, Acore::ChatCommands::Console::No},
            {"learn all", HandleLearnAllCommand, SEC_GAMEMASTER, Acore::ChatCommands::Console::No},
            {"teleport", HandleTeleportCommand, SEC_PLAYER, Acore::ChatCommands::Console::No},
            {"remove", HandleRemoveCommand, SEC_GAMEMASTER, Acore::ChatCommands::Console::No},
            {"remove all", HandleRemoveAllCommand, SEC_GAMEMASTER, Acore::ChatCommands::Console::No},
            {"cd", HandleCooldownCommand, SEC_PLAYER, Acore::ChatCommands::Console::No},
            {"cd all", HandleCooldownListCommand, SEC_PLAYER, Acore::ChatCommands::Console::No},
            {"cd reset", HandleCooldownResetCommand, SEC_GAMEMASTER, Acore::ChatCommands::Console::No},
            {"cd reset all", HandleCooldownResetAllCommand, SEC_GAMEMASTER, Acore::ChatCommands::Console::No}};

    static Acore::ChatCommands::ChatCommandTable commandTable =
        {
            {"tr", teleportTable}};

    return commandTable;
}

std::string TravelersRecallCommandScript::BuildCooldownRemainingText(uint32 remainingSeconds)
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

bool TravelersRecallCommandScript::HandleTeleportToFriend(Player *player, ChatHandler *handler, uint32 locationId, uint32 cooldown)
{
    ObjectGuid targetGuid = player->GetTarget();

    if (!targetGuid)
    {
        handler->SendSysMessage("Traveler's Recall: No target selected.");
        return true;
    }

    Player *selectedPlayer = ObjectAccessor::FindConnectedPlayer(targetGuid);

    if (!selectedPlayer || selectedPlayer == player)
    {
        handler->SendSysMessage("Traveler's Recall: You must select another player.");
        return true;
    }

    if (!selectedPlayer->IsInWorld())
    {
        ChatHandler(player->GetSession()).SendSysMessage("Traveler's Recall: Target must be in the overworld.");
        return true;
    }

    if (cooldown == 0 && sConfigMgr->GetOption<bool>("TravelersRecall.UseDefaultCooldown", false))
    {
        cooldown = sConfigMgr->GetOption<uint32>("TravelersRecall.DefaultCooldown", 0);
    }

    if (cooldown > 0)
    {
        uint32 remaining = TravelersRecall::GetCooldownRemaining(player, locationId);

        if (remaining > 0)
        {
            std::string remainingText = BuildCooldownRemainingText(remaining);

            handler->PSendSysMessage(
                "Traveler's Recall: cooldown remaining to teleport to a friend: {}.",
                remainingText);

            return true;
        }
    }

    std::string result = TravelersRecall::CanPlayerTeleportToFried(player, selectedPlayer, handler, locationId);
    if (result != "OK")
    {
        ChatHandler(player->GetSession()).SendSysMessage(result);
        return true;
    }

    player->TeleportTo(
        selectedPlayer->GetMapId(),
        selectedPlayer->GetPositionX(),
        selectedPlayer->GetPositionY(),
        selectedPlayer->GetPositionZ() + 0.25f,
        player->GetOrientation(),
        TELE_TO_GM_MODE,
        selectedPlayer);

    if (cooldown > 0)
    {
        uint32 cooldownEnd = duration_cast<Seconds>(GameTime::GetSystemTime().time_since_epoch()).count() + cooldown;

        CharacterDatabase.Execute(
            "UPDATE custom_travelers_recall_unlocks SET cooldown_end = {} WHERE guid = {} AND location_id = {}",
            cooldownEnd,
            player->GetGUID().GetCounter(),
            locationId);
    }

    handler->PSendSysMessage("Traveler's Recall: teleported.");

    return true;
}

bool TravelersRecallCommandScript::SendListForPlayer(Player *player)
{
    QueryResult unlocks = CharacterDatabase.Query(
        "SELECT location_id FROM custom_travelers_recall_unlocks WHERE guid = {}",
        player->GetGUID().GetCounter());

    if (!unlocks)
    {
        return true;
    }

    bool canTeleportToFriend = sConfigMgr->GetOption<bool>("TravelersRecall.TeleportToFriendEnabled", false);

    do
    {
        Field *unlockFields = unlocks->Fetch();

        uint32 locationId = unlockFields[0].Get<uint32>();

        QueryResult location = WorldDatabase.Query(
            "SELECT area_id, name, map_id, icon FROM custom_travelers_recall_locations WHERE id = {}",
            locationId);

        if (!location)
        {
            continue;
        }

        Field *locationFields = location->Fetch();

        uint32 areaId = locationFields[0].Get<uint32>();
        std::string locationName = locationFields[1].Get<std::string>();
        uint32 mapId = locationFields[2].Get<uint32>();
        std::string icon = locationFields[3].Get<std::string>();

        if (areaId == -1 && mapId == -1 && canTeleportToFriend == false)
        {
            continue;
        }

        std::string message = Acore::StringFormat(
            "TR_LIST:{}:{}:{}",
            locationId,
            locationName,
            icon);

        ChatHandler(player->GetSession()).SendSysMessage(message.c_str());

    } while (unlocks->NextRow());

    return true;
}

bool TravelersRecallCommandScript::HandleListCommand(ChatHandler *handler, char const * /*args*/)
{
    Player *player = handler->GetPlayer();
    return SendListForPlayer(player);
}

bool TravelersRecallCommandScript::HandleTeleportCommand(ChatHandler *handler, char const *args)
{
    Player *player = handler->GetPlayer();
    uint32 locationId = atoi(args);

    if (!TravelersRecall::CanPlayerTeleport(player, handler, locationId))
    {
        return true;
    }

    QueryResult location = WorldDatabase.Query(
        "SELECT area_id, map_id, position_x, position_y, position_z, orientation, name, cooldown FROM custom_travelers_recall_locations WHERE id = {}",
        locationId);

    if (!location)
    {
        handler->SendSysMessage("Traveler's Recall: location not found.");
        return false;
    }

    Field *fields = location->Fetch();
    int32 areaId = fields[0].Get<int32>();
    int32 mapId = fields[1].Get<int32>();
    uint32 cooldown = fields[7].Get<uint32>();

    bool canTeleportToFriend = sConfigMgr->GetOption<bool>("TravelersRecall.TeleportToFriendEnabled", false);

    if (areaId == -1 && mapId == -1 && canTeleportToFriend == true)
    {
        return HandleTeleportToFriend(player, handler, locationId, cooldown);
    }

    float x = fields[2].Get<float>();
    float y = fields[3].Get<float>();
    float z = fields[4].Get<float>();
    float o = fields[5].Get<float>();
    std::string locationName = fields[6].Get<std::string>();

    if (cooldown == 0 && sConfigMgr->GetOption<bool>("TravelersRecall.UseDefaultCooldown", false))
    {
        cooldown = sConfigMgr->GetOption<uint32>("TravelersRecall.DefaultCooldown", 0);
    }

    if (cooldown > 0)
    {
        uint32 remaining = TravelersRecall::GetCooldownRemaining(player, locationId);

        if (remaining > 0)
        {
            std::string remainingText = BuildCooldownRemainingText(remaining);

            handler->PSendSysMessage(
                "Traveler's Recall: cooldown remaining for {}: {}.",
                locationName,
                remainingText);

            return true;
        }
    }

    if (!player->TeleportTo(mapId, x, y, z, o))
    {
        handler->SendSysMessage("Traveler's Recall: teleportation failed.");
        return false;
    }

    bool teleportPartyBots = sConfigMgr->GetOption<bool>("TravelersRecall.TeleportPartyBots", false);
    bool teleportHeadlessClients = sConfigMgr->GetOption<bool>("TravelersRecall.TeleportHeadlessClient", false);

    if (teleportPartyBots || teleportHeadlessClients)
    {
        if (Group *group = player->GetGroup())
        {
            if (!group->IsLeader(player->GetGUID()))
            {
                handler->SendSysMessage(
                    "Traveler's Recall: only the group leader can teleport party bots or "
                    "headless clients.");
            }
            else
            {
                for (GroupReference *itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
                {
                    Player *partyMember = itr->GetSource();

                    if (!partyMember)
                    {
                        continue;
                    }

                    WorldSession *session = partyMember->GetSession();
                    if (!session)
                    {
                        continue;
                    }

                    if (!partyMember->IsInWorld())
                    {
                        continue;
                    }

                    bool shouldTeleport = (teleportPartyBots && IsBotSession(session)) || (teleportHeadlessClients && IsHeadlessClientSession(session));

                    if (shouldTeleport)
                    {
                        if (!partyMember->TeleportTo(mapId, x, y, z, o))
                        {
                            handler->SendSysMessage("Traveler's Recall: party member teleportation failed.");
                        }
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

bool TravelersRecallCommandScript::HandleLearnCommand(ChatHandler *handler, char const *args)
{
    Player *target = handler->getSelectedPlayerOrSelf();

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

    Field *locFields = result->Fetch();

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

bool TravelersRecallCommandScript::HandleLearnAllCommand(ChatHandler *handler, char const *args)
{
    Player *target = handler->getSelectedPlayerOrSelf();

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
        Field *fields = result->Fetch();

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

bool TravelersRecallCommandScript::HandleRemoveCommand(ChatHandler *handler, char const *args)
{
    Player *target = handler->getSelectedPlayerOrSelf();

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

bool TravelersRecallCommandScript::HandleRemoveAllCommand(ChatHandler *handler, char const *args)
{
    Player *target = handler->getSelectedPlayerOrSelf();

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
        Field *fields = result->Fetch();
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

bool TravelersRecallCommandScript::HandleCooldownCommand(ChatHandler *handler, char const *args)
{
    Player *player = handler->GetPlayer();

    if (!args || !*args)
    {
        handler->SendSysMessage("Traveler's Recall: location id required.");
        return true;
    }

    uint32 locationId = atoi(args);

    if (locationId == 0)
    {
        handler->SendSysMessage("Traveler's Recall: invalid location id.");
        return true;
    }

    if (!TravelersRecall::HasUnlockedLocation(player, locationId))
    {
        handler->SendSysMessage("Traveler's Recall: location is not unlocked.");
        return true;
    }

    QueryResult locationName = WorldDatabase.Query(
        "SELECT name FROM custom_travelers_recall_locations WHERE id = {}",
        locationId);

    if (!locationName)
    {
        handler->SendSysMessage("Traveler's Recall: location not found.");
        return true;
    }

    std::string name = locationName->Fetch()[0].Get<std::string>();
    uint32 remaining = TravelersRecall::GetCooldownRemaining(player, locationId);
    std::string cooldownRemaining = BuildCooldownRemainingText(remaining);

    handler->PSendSysMessage(
        "Traveler's Recall: cooldown remaining for {}: {}.",
        name,
        cooldownRemaining);

    return true;
}

bool TravelersRecallCommandScript::HandleCooldownListCommand(ChatHandler *handler, char const *args)
{
    Player *player = handler->getSelectedPlayerOrSelf();

    QueryResult unlocks = CharacterDatabase.Query(
        "SELECT location_id FROM custom_travelers_recall_unlocks WHERE guid = {} AND cooldown_end > UNIX_TIMESTAMP()",
        player->GetGUID().GetCounter());

    if (!unlocks)
    {
        return true;
    }

    do
    {
        Field *unlockFields = unlocks->Fetch();

        uint32 locationId = unlockFields[0].Get<uint32>();

        QueryResult location = WorldDatabase.Query(
            "SELECT name FROM custom_travelers_recall_locations WHERE id = {}",
            locationId);

        if (!location)
        {
            continue;
        }

        Field *locationFields = location->Fetch();

        std::string locationName = locationFields[0].Get<std::string>();

        QueryResult cooldownResult = CharacterDatabase.Query(
            "SELECT cooldown_end FROM custom_travelers_recall_unlocks WHERE guid = {} AND location_id = {}",
            player->GetGUID().GetCounter(),
            locationId);

        if (!cooldownResult)
        {
            continue;
        }

        uint32 remaining = TravelersRecall::GetCooldownRemaining(player, locationId);
        std::string cooldownRemaining = BuildCooldownRemainingText(remaining);

        handler->PSendSysMessage(
            "Traveler's Recall: cooldown remaining for {}: {}.",
            locationName,
            cooldownRemaining);

    } while (unlocks->NextRow());
    return true;
}

bool TravelersRecallCommandScript::HandleCooldownResetCommand(ChatHandler *handler, char const *args)
{
    Player *player = handler->getSelectedPlayerOrSelf();

    if (!player)
    {
        return false;
    }

    if (!args || !*args)
    {
        handler->SendSysMessage("Traveler's Recall: location id required.");
        return true;
    }

    uint32 locationId = atoi(args);

    if (locationId == 0)
    {
        handler->SendSysMessage("Traveler's Recall: invalid location id.");
        return true;
    }

    CharacterDatabase.Execute(
        "UPDATE custom_travelers_recall_unlocks SET cooldown_end = 0 WHERE guid = {} AND location_id = {}",
        player->GetGUID().GetCounter(),
        locationId);

    QueryResult locationName = WorldDatabase.Query(
        "SELECT name FROM custom_travelers_recall_locations WHERE id = {}",
        locationId);

    if (!locationName)
    {
        handler->SendSysMessage("Traveler's Recall: location not found.");
        return true;
    }

    std::string name = locationName->Fetch()[0].Get<std::string>();

    handler->PSendSysMessage(
        "Traveler's Recall: cooldown for {} reset.",
        name);

    return true;
}

bool TravelersRecallCommandScript::HandleCooldownResetAllCommand(ChatHandler *handler, char const *args)
{
    Player *player = handler->getSelectedPlayerOrSelf();

    if (!player)
    {
        return false;
    }

    CharacterDatabase.Execute(
        "UPDATE custom_travelers_recall_unlocks SET cooldown_end = 0 WHERE guid = {}",
        player->GetGUID().GetCounter());

    handler->PSendSysMessage(
        "Traveler's Recall: all cooldowns reset.");

    return true;
}
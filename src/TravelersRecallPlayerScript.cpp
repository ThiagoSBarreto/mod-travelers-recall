#include "TravelersRecallPlayerScript.h"
#include <sstream>

namespace TravelersRecall
{
    void UnlockLocations(Player *player, std::string const &locations)
    {
        std::stringstream locationStream(locations);
        std::string locationToken;

        while (std::getline(locationStream, locationToken, ','))
        {
            std::stringstream tokenStream(locationToken);
            uint32 locationId = 0;

            if (!(tokenStream >> locationId) || locationId == 0)
            {
                continue;
            }

            QueryResult locationResult = WorldDatabase.Query(
                "SELECT id FROM custom_travelers_recall_locations WHERE area_id = {}",
                locationId);

            if (!locationResult)
            {
                continue;
            }

            uint32 customLocationId = locationResult->Fetch()[0].Get<uint32>();

            CharacterDatabase.Execute(
                "INSERT IGNORE INTO custom_travelers_recall_unlocks (guid, location_id, unlocked_at, cooldown_end) VALUES ({}, {}, NOW(), 0)",
                player->GetGUID().GetCounter(),
                customLocationId);
        }
    }

    std::string GetRaceTeleportConfig(Player *player)
    {
        switch (player->getRace())
        {
        case RACE_HUMAN:
            return "TravelersRecall.InitialHumanTeleports";
        case RACE_DWARF:
            return "TravelersRecall.InitialDwarfTeleports";
        case RACE_NIGHTELF:
            return "TravelersRecall.InitialNightElfTeleports";
        case RACE_GNOME:
            return "TravelersRecall.InitialGnomeTeleports";
        case RACE_DRAENEI:
            return "TravelersRecall.InitialDraeneiTeleports";

        case RACE_ORC:
            return "TravelersRecall.InitialOrcTeleports";
        case RACE_UNDEAD_PLAYER:
            return "TravelersRecall.InitialUndeadTeleports";
        case RACE_TAUREN:
            return "TravelersRecall.InitialTaurenTeleports";
        case RACE_TROLL:
            return "TravelersRecall.InitialTrollTeleports";
        case RACE_BLOODELF:
            return "TravelersRecall.InitialBloodElfTeleports";
        default:
            return {};
        }
    }

    void AddTeleportToFriend(Player *player)
    {
        QueryResult locationResult = WorldDatabase.Query(
            "SELECT id FROM custom_travelers_recall_locations WHERE area_id = -1 && map_id = -1");

        if (!locationResult)
        {
            return;
        }

        Field *locationFields = locationResult->Fetch();
        uint32 locationId = locationFields[0].Get<uint32>();

        CharacterDatabase.Execute(
            "INSERT INTO custom_travelers_recall_unlocks (guid, location_id) VALUES ({}, {})",
            player->GetGUID().GetCounter(),
            locationId);
    }
}

TravelersRecallPlayerScript::TravelersRecallPlayerScript()
    : PlayerScript("TravelersRecallPlayerScript")
{
}

void TravelersRecallPlayerScript::OnPlayerUpdateArea(Player *player, uint32 oldArea, uint32 newArea)
{
    if (!sConfigMgr->GetOption<bool>("TravelersRecall.Enable", false))
    {
        return;
    }

    if (player->isDead())
    {
        return;
    }

    if (oldArea == newArea)
    {
        return;
    }

    QueryResult locationResult = WorldDatabase.Query(
        "SELECT id, name, faction, icon, required_level FROM custom_travelers_recall_locations WHERE area_id = {}",
        newArea);

    if (!locationResult)
    {
        return;
    }

    Field *locationFields = locationResult->Fetch();

    uint32 locationId = locationFields[0].Get<uint32>();
    std::string locationName = locationFields[1].Get<std::string>();
    uint8 faction = locationFields[2].Get<uint8>();
    std::string icon = locationFields[3].Get<std::string>();
    uint32 requiredLevel = locationFields[4].Get<uint32>();

    uint32 useRequiredLevel = sConfigMgr->GetOption<uint32>("TravelersRecall.UseRequiredLevel", 0);

    if (useRequiredLevel == 1)
    {
        if (player->GetLevel() < requiredLevel)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("You need to be at least Level {} to unlock {}", requiredLevel, locationName);
            return;
        }
    }

    if (!player->IsGameMaster())
    {
        TeamId playerTeam = player->GetTeamId();

        if (faction == 1 && playerTeam != TEAM_ALLIANCE)
        {
            return;
        }

        if (faction == 2 && playerTeam != TEAM_HORDE)
        {
            return;
        }
    }

    QueryResult unlockResult = CharacterDatabase.Query(
        "SELECT 1 FROM custom_travelers_recall_unlocks WHERE guid = {} AND location_id = {}",
        player->GetGUID().GetCounter(),
        locationId);

    if (unlockResult)
    {
        return;
    }

    CharacterDatabase.Execute(
        "INSERT INTO custom_travelers_recall_unlocks (guid, location_id) VALUES ({}, {})",
        player->GetGUID().GetCounter(),
        locationId);

    ChatHandler(player->GetSession()).PSendSysMessage("New location unlocked: {}", locationName);

    TravelersRecall::SendUnlockMessage(player, locationId, locationName, icon);

    LOG_INFO(
        "module",
        "Traveler's Recall: player {} unlocked {}",
        player->GetName(),
        locationName);
}

void TravelersRecallPlayerScript::OnPlayerDelete(ObjectGuid guid, uint32 account)
{
    CharacterDatabase.Execute(
        "DELETE FROM custom_travelers_recall_unlocks WHERE guid = {}",
        guid.GetCounter());
}

void TravelersRecallPlayerScript::OnPlayerCreate(Player *player)
{
    TravelersRecall::AddTeleportToFriend(player);
    
    if (!sConfigMgr->GetOption<bool>("TravelersRecall.InitialTeleportLocationsEnabled", false))
    {
        return;
    }

    std::string factionConfig;

    if (player->GetTeamId() == TEAM_ALLIANCE)
    {
        factionConfig = "TravelersRecall.InitialAllianceTeleports";
    }
    else if (player->GetTeamId() == TEAM_HORDE)
    {
        factionConfig = "TravelersRecall.InitialHordeTeleports";
    }

    TravelersRecall::UnlockLocations(player, sConfigMgr->GetOption<std::string>(factionConfig, ""));

    std::string raceConfig = TravelersRecall::GetRaceTeleportConfig(player);

    TravelersRecall::UnlockLocations(player, sConfigMgr->GetOption<std::string>(raceConfig, ""));
}

void TravelersRecallPlayerScript::OnPlayerLogin(Player *player)
{
    if (!sConfigMgr->GetOption<bool>("TravelersRecall.WelcomeMessage", false))
    {
        return;
    }
    ChatHandler(player->GetSession()).PSendSysMessage("This servers uses Traveler's Recall module");
}

void TravelersRecallPlayerScript::OnPlayerFirstLogin(Player *player)
{
    if (!sConfigMgr->GetOption<bool>("TravelersRecall.StartWithAllLocationsUnlocked", false))
    {
        return;
    }

    uint8 faction = player->GetTeamId() == TEAM_ALLIANCE ? 1 : 2;
    QueryResult locationResult = WorldDatabase.Query(
        "SELECT id FROM custom_travelers_recall_locations WHERE faction = 0 OR faction = {}",
        faction);

    if (!locationResult)
    {
        return;
    }

    do
    {
        uint32 locationId = locationResult->Fetch()[0].Get<uint32>();
        CharacterDatabase.Execute(
            "INSERT IGNORE INTO custom_travelers_recall_unlocks (guid, location_id, unlocked_at, cooldown_end) "
            "VALUES ({}, {}, NOW(), 0)",
            player->GetGUID().GetCounter(),
            locationId);
    } while (locationResult->NextRow());
}
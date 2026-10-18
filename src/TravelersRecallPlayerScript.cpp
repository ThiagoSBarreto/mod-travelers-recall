#include "TravelersRecallPlayerScript.h"

#include "Chat.h"
#include "Config.h"
#include "DBCStores.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "Player.h"

#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace
{
    struct RecallLocation
    {
        uint32 id = 0;
        uint32 areaId = 0;
        std::string name;
        uint8 faction = 0;
        std::string icon;
        uint32 requiredLevel = 0;
    };

    // World locations are effectively static while the worldserver is running.
    // Load them once, then use this cache for area-change events and login helpers.
    std::unordered_map<uint32, std::vector<RecallLocation>> LocationsByArea;
    std::vector<RecallLocation> AllLocations;
    uint32 TeleportToFriendLocationId = 0;
    bool LocationsLoaded = false;

    // Per-character unlock cache. Accessed from world-thread PlayerScript callbacks.
    std::unordered_map<uint32, std::unordered_set<uint32>> UnlocksByCharacter;
    std::unordered_set<uint32> LoadedCharacterUnlocks;

    void LoadLocationCache()
    {
        LocationsByArea.clear();
        AllLocations.clear();
        TeleportToFriendLocationId = 0;

        QueryResult result = WorldDatabase.Query(
            "SELECT id, area_id, name, faction, icon, required_level "
            "FROM custom_travelers_recall_locations");

        if (result)
        {
            do
            {
                Field *fields = result->Fetch();

                RecallLocation location;
                location.id = fields[0].Get<uint32>();
                int32 rawAreaId = fields[1].Get<int32>();
                location.areaId = rawAreaId < 0 ? 0 : static_cast<uint32>(rawAreaId);
                location.name = fields[2].Get<std::string>();
                location.faction = fields[3].Get<uint8>();
                location.icon = fields[4].Get<std::string>();
                location.requiredLevel = fields[5].Get<uint32>();

                // The special Teleport to Friend row uses area_id = -1 and map_id = -1.
                if (rawAreaId >= 0)
                {
                    LocationsByArea[location.areaId].push_back(location);
                }

                AllLocations.push_back(location);
            } while (result->NextRow());
        }

        QueryResult friendResult = WorldDatabase.Query(
            "SELECT id FROM custom_travelers_recall_locations "
            "WHERE area_id = -1 AND map_id = -1 LIMIT 1");

        if (friendResult)
        {
            TeleportToFriendLocationId = friendResult->Fetch()[0].Get<uint32>();
        }

        LocationsLoaded = true;
    }

    void EnsureLocationCacheLoaded()
    {
        if (!LocationsLoaded)
        {
            LoadLocationCache();
        }
    }

    void EnsureCharacterUnlockCacheLoaded(Player *player)
    {
        uint32 guid = player->GetGUID().GetCounter();
        if (LoadedCharacterUnlocks.find(guid) != LoadedCharacterUnlocks.end())
        {
            return;
        }

        std::unordered_set<uint32> &unlocks = UnlocksByCharacter[guid];
        QueryResult result = CharacterDatabase.Query(
            "SELECT location_id FROM custom_travelers_recall_unlocks WHERE guid = {}",
            guid);

        if (result)
        {
            do
            {
                unlocks.insert(result->Fetch()[0].Get<uint32>());
            } while (result->NextRow());
        }

        LoadedCharacterUnlocks.insert(guid);
    }

    bool IsUnlocked(Player *player, uint32 locationId)
    {
        EnsureCharacterUnlockCacheLoaded(player);
        uint32 guid = player->GetGUID().GetCounter();
        return UnlocksByCharacter[guid].find(locationId) != UnlocksByCharacter[guid].end();
    }

    void InsertUnlock(Player *player, uint32 locationId, bool includeTimestamps = false)
    {
        uint32 guid = player->GetGUID().GetCounter();
        EnsureCharacterUnlockCacheLoaded(player);

        // Update memory before issuing the asynchronous database write. This prevents
        // duplicate messages/inserts if another area event arrives before the write runs.
        if (!UnlocksByCharacter[guid].insert(locationId).second)
        {
            return;
        }

        if (includeTimestamps)
        {
            CharacterDatabase.Execute(
                "INSERT IGNORE INTO custom_travelers_recall_unlocks "
                "(guid, location_id, unlocked_at, cooldown_end) "
                "VALUES ({}, {}, NOW(), 0)",
                guid, locationId);
        }
        else
        {
            CharacterDatabase.Execute(
                "INSERT IGNORE INTO custom_travelers_recall_unlocks (guid, location_id) "
                "VALUES ({}, {})",
                guid, locationId);
        }
    }

    std::vector<uint32> ParseLocationIds(std::string const &locations)
    {
        std::vector<uint32> ids;
        std::stringstream locationStream(locations);
        std::string token;

        while (std::getline(locationStream, token, ','))
        {
            std::stringstream tokenStream(token);
            uint32 id = 0;
            if (tokenStream >> id && id != 0)
            {
                ids.push_back(id);
            }
        }

        return ids;
    }
}

namespace TravelersRecall
{
    void UnlockLocations(Player *player, std::string const &locations)
    {
        EnsureLocationCacheLoaded();

        std::vector<uint32> areaIds = ParseLocationIds(locations);
        if (areaIds.empty())
        {
            return;
        }

        // Resolve all configured area IDs with one world-database query.
        std::ostringstream query;
        query << "SELECT id, area_id FROM custom_travelers_recall_locations WHERE area_id IN (";
        for (size_t i = 0; i < areaIds.size(); ++i)
        {
            if (i != 0)
                query << ',';
            query << areaIds[i];
        }
        query << ')';

        std::unordered_set<uint32> locationIds;
        QueryResult result = WorldDatabase.Query(query.str());
        if (result)
        {
            do
            {
                Field *fields = result->Fetch();
                locationIds.insert(fields[0].Get<uint32>());
            } while (result->NextRow());
        }

        for (uint32 locationId : locationIds)
        {
            InsertUnlock(player, locationId, true);
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
        EnsureLocationCacheLoaded();

        if (TeleportToFriendLocationId == 0)
        {
            return;
        }

        InsertUnlock(player, TeleportToFriendLocationId);
    }
}

TravelersRecallPlayerScript::TravelersRecallPlayerScript()
    : PlayerScript("TravelersRecallPlayerScript")
{
}

void TravelersRecallPlayerScript::OnPlayerUpdateArea(Player *player, uint32 oldArea, uint32 newArea)
{
    if (IsBotSession(player->GetSession()))
    {
        return;
    }

    if (!sConfigMgr->GetOption<bool>("TravelersRecall.Enable", false))
    {
        return;
    }

    if (player->isDead() || oldArea == newArea)
    {
        return;
    }

    EnsureLocationCacheLoaded();

    AreaTableEntry const *areaEntry = sAreaTableStore.LookupEntry(newArea);
    uint32 zoneId = areaEntry ? areaEntry->zone : 0;

    std::vector<RecallLocation const *> matchingLocations;
    auto addLocations = [&matchingLocations](uint32 areaId)
    {
        auto itr = LocationsByArea.find(areaId);
        if (itr == LocationsByArea.end())
        {
            return;
        }

        for (RecallLocation const &location : itr->second)
        {
            matchingLocations.push_back(&location);
        }
    };

    if (zoneId != 0)
    {
        addLocations(zoneId);
    }
    else
    {
        addLocations(newArea);
    }

    if (matchingLocations.empty())
    {
        return;
    }

    EnsureCharacterUnlockCacheLoaded(player);

    uint32 useRequiredLevel = sConfigMgr->GetOption<uint32>("TravelersRecall.UseRequiredLevel", 0);
    TeamId playerTeam = player->GetTeamId();

    for (RecallLocation const *location : matchingLocations)
    {
        if (useRequiredLevel == 1 && player->GetLevel() < location->requiredLevel)
        {
            continue;
        }

        if (!player->IsGameMaster())
        {
            if (location->faction == 1 && playerTeam != TEAM_ALLIANCE)
            {
                continue;
            }

            if (location->faction == 2 && playerTeam != TEAM_HORDE)
            {
                continue;
            }
        }

        if (IsUnlocked(player, location->id))
        {
            continue;
        }

        InsertUnlock(player, location->id);

        ChatHandler(player->GetSession()).PSendSysMessage("New location unlocked: {}", location->name);

        TravelersRecall::SendUnlockMessage(player, location->id, location->name, location->icon);
    }
}

void TravelersRecallPlayerScript::OnPlayerDelete(ObjectGuid guid, uint32 /*account*/)
{
    uint32 guidCounter = guid.GetCounter();

    CharacterDatabase.Execute(
        "DELETE FROM custom_travelers_recall_unlocks WHERE guid = {}",
        guidCounter);

    UnlocksByCharacter.erase(guidCounter);
    LoadedCharacterUnlocks.erase(guidCounter);
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

    if (!factionConfig.empty())
    {
        TravelersRecall::UnlockLocations(player, sConfigMgr->GetOption<std::string>(factionConfig, ""));
    }

    std::string raceConfig = TravelersRecall::GetRaceTeleportConfig(player);
    if (!raceConfig.empty())
    {
        TravelersRecall::UnlockLocations(player, sConfigMgr->GetOption<std::string>(raceConfig, ""));
    }
}

void TravelersRecallPlayerScript::OnPlayerLogin(Player *player)
{
    EnsureCharacterUnlockCacheLoaded(player);
    TravelersRecall::AddTeleportToFriend(player);

    if (sConfigMgr->GetOption<bool>("TravelersRecall.WelcomeMessage", false))
    {
        ChatHandler(player->GetSession()).PSendSysMessage("This server uses Traveler's Recall module");
    }
}

void TravelersRecallPlayerScript::OnPlayerLogout(Player *player)
{
    uint32 guid = player->GetGUID().GetCounter();
    UnlocksByCharacter.erase(guid);
    LoadedCharacterUnlocks.erase(guid);
}

void TravelersRecallPlayerScript::OnPlayerFirstLogin(Player *player)
{
    if (!sConfigMgr->GetOption<bool>("TravelersRecall.StartWithAllLocationsUnlocked", false))
    {
        return;
    }

    EnsureLocationCacheLoaded();
    EnsureCharacterUnlockCacheLoaded(player);

    uint8 faction = player->GetTeamId() == TEAM_ALLIANCE ? 1 : 2;

    for (RecallLocation const &location : AllLocations)
    {
        if (location.areaId == 0)
        {
            continue;
        }

        if (location.faction != 0 && location.faction != faction)
        {
            continue;
        }

        InsertUnlock(player, location.id, true);
    }
}

void TravelersRecallPlayerScript::CacheUnlock(Player *player, uint32 locationId)
{
    if (!player)
    {
        return;
    }

    EnsureCharacterUnlockCacheLoaded(player);

    uint32 guid = player->GetGUID().GetCounter();
    UnlocksByCharacter[guid].insert(locationId);
}

void TravelersRecallPlayerScript::CacheRemove(Player *player, uint32 locationId)
{
    if (!player)
    {
        return;
    }

    EnsureCharacterUnlockCacheLoaded(player);

    uint32 guid = player->GetGUID().GetCounter();
    UnlocksByCharacter[guid].erase(locationId);
}

void TravelersRecallPlayerScript::CacheRemoveAll(Player *player)
{
    if (!player)
    {
        return;
    }

    EnsureCharacterUnlockCacheLoaded(player);

    uint32 guid = player->GetGUID().GetCounter();
    UnlocksByCharacter[guid].clear();
}

template <typename Session>
bool IsBotSessionImpl(Session const *session)
{
    if constexpr (requires { session->IsBot(); })
    {
        return session->IsBot();
    }
    else
    {
        return false;
    }
}

bool TravelersRecallPlayerScript::IsBotSession(WorldSession const *session)
{
    return session && IsBotSessionImpl(session);
}

template <typename Session>
bool IsHeadlessClientSessionImpl(Session const *session)
{
    if constexpr (requires { session->IsHeadlessClient(); })
    {
        return session->IsHeadlessClient();
    }
    else
    {
        return false;
    }
}

bool TravelersRecallPlayerScript::IsHeadlessClientSession(WorldSession const *session)
{
    return session && IsHeadlessClientSessionImpl(session);
}

#include "TravelersRecall.h"
#include "TravelersRecallPlayerScript.h"
#include "TravelersRecallCommandScript.h"
#include "TravelersRecallSpellScript.h"

#include "DBCStores.h"

#include <algorithm>

namespace
{
    // Sort order used by the client addon: 1 Eastern Kingdoms, 2 Kalimdor,
    // 3 Outland, 4 Northrend, 9 anything else.
    uint32 GetContinentIndex(uint32 mapId, uint32 zoneId)
    {
        // The Burning Crusade starting zones live on map 530 (Outland) but are
        // geographically part of the old continents.
        switch (zoneId)
        {
            case 3430: // Eversong Woods
            case 3433: // Ghostlands
            case 3487: // Silvermoon City
            case 4080: // Isle of Quel'Danas
                return 1;
            case 3524: // Azuremyst Isle
            case 3525: // Bloodmyst Isle
            case 3557: // The Exodar
                return 2;
            default:
                break;
        }

        switch (mapId)
        {
            case 0:   return 1;
            case 1:   return 2;
            case 530: return 3;
            case 571: return 4;
            default:  return 9;
        }
    }

    std::string GetAreaName(AreaTableEntry const* area, LocaleConstant locale)
    {
        char const* name = area->area_name[locale];
        if (!name || !*name)
        {
            name = area->area_name[LOCALE_enUS];
        }

        std::string result = name ? name : "";
        // ':' is the field separator of the addon protocol.
        std::replace(result.begin(), result.end(), ':', ' ');
        return result;
    }
}

namespace TravelersRecall
{
    void SendAddonMessage(Player* player, std::string const& message)
    {
        ChatHandler(player->GetSession()).SendSysMessage(message);
    }

    // TR_META:<locationId>:<continentIndex>:<zoneName>
    // Lets the addon group and sort locations. Older addon versions ignore it.
    void SendLocationMeta(Player* player, uint32 locationId, int32 areaId)
    {
        if (areaId <= 0)
        {
            return;
        }

        AreaTableEntry const* area = sAreaTableStore.LookupEntry(static_cast<uint32>(areaId));
        if (!area)
        {
            return;
        }

        AreaTableEntry const* zone = area->zone ? sAreaTableStore.LookupEntry(area->zone) : area;
        if (!zone)
        {
            zone = area;
        }

        std::string message = Acore::StringFormat(
            "TR_META:{}:{}:{}",
            locationId,
            GetContinentIndex(area->mapid, zone->ID),
            GetAreaName(zone, player->GetSession()->GetSessionDbcLocale()));

        SendAddonMessage(player, message);
    }

    // TR_CD:<locationId>:<remainingSeconds>
    // locationId 0 means "all locations" (used by .tr cd reset all), 0 seconds clears the cooldown.
    void SendCooldownMessage(Player* player, uint32 locationId, uint32 remainingSeconds)
    {
        SendAddonMessage(player, Acore::StringFormat("TR_CD:{}:{}", locationId, remainingSeconds));
    }

    void SendUnlockMessage(Player* player, uint32 locationId, int32 areaId, std::string const& locationName, std::string const& icon)
    {
        SendLocationMeta(player, locationId, areaId);

        std::string message = Acore::StringFormat(
            "TR_UNLOCK:{}:{}:{}",
            locationId,
            locationName,
            icon);

        SendAddonMessage(player, message);
    }

    void SendRemoveMessage(Player* player, uint32 locationId)
    {
        std::string message = Acore::StringFormat(
            "TR_REMOVE:{}",
            locationId);

        SendAddonMessage(player, message);
    }
}

void Addmod_travelers_recallScripts()
{
    new TravelersRecallPlayerScript();
    new TravelersRecallCommandScript();
    AddTravelersRecallSpellScripts();
}

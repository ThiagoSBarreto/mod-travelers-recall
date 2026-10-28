#include "TravelersRecallService.h"

namespace TravelersRecall
{
    bool HasUnlockedLocation(Player* player, uint32 locationId)
    {
        QueryResult result = CharacterDatabase.Query(
            "SELECT 1 FROM custom_travelers_recall_unlocks WHERE guid = {} AND location_id = {}",
            player->GetGUID().GetCounter(),
            locationId);

        return result != nullptr;
    }

    uint32 GetCooldownRemaining(Player* player, uint32 locationId)
    {
        QueryResult result = CharacterDatabase.Query(
            "SELECT cooldown_end FROM custom_travelers_recall_unlocks WHERE guid = {} AND location_id = {}",
            player->GetGUID().GetCounter(),
            locationId);

        if (!result)
        {
            return 0;
        }

        uint32 cooldownEnd = result->Fetch()[0].Get<uint32>();
        uint32 now = duration_cast<Seconds>(GameTime::GetSystemTime().time_since_epoch()).count();

        return cooldownEnd > now ? cooldownEnd - now : 0;
    }

    bool CanPlayerTeleport(Player* player, ChatHandler* handler, uint32 locationId)
    {
        if (player->GetTransport())
        {
            handler->SendSysMessage("Traveler's Recall: cannot teleport while on a transport.");
            return false;
        }

        if (player->IsGameMaster())
        {
            return true;
        }

        if (!HasUnlockedLocation(player, locationId))
        {
            handler->SendSysMessage("Traveler's Recall: location not unlocked.");
            return false;
        }

        bool allowInFlight = sConfigMgr->GetOption<bool>("TravelersRecall.CanTeleport.InFlight", false);
        if (player->IsInFlight() && !allowInFlight)
        {
            handler->SendSysMessage("Traveler's Recall: cannot teleport while flying.");
            return false;
        }

        bool allowWhileStealthed = sConfigMgr->GetOption<bool>("TravelersRecall.CanTeleport.WhileStealthed", false);
        if (player->HasStealthAura() && !allowWhileStealthed)
        {
            handler->SendSysMessage("Traveler's Recall: cannot teleport while stealthed.");
            return false;
        }

        bool allowInCombat = sConfigMgr->GetOption<bool>("TravelersRecall.CanTeleport.InCombat", false);
        if (player->IsInCombat() && !allowInCombat)
        {
            handler->SendSysMessage("Traveler's Recall: cannot teleport while in combat.");
            return false;
        }

        bool allowWhileDead = sConfigMgr->GetOption<bool>("TravelersRecall.CanTeleport.WhileDead", false);
        if (player->isDead() && !allowWhileDead)
        {
            handler->SendSysMessage("Traveler's Recall: cannot teleport while dead.");
            return false;
        }

        bool allowInBattleground = sConfigMgr->GetOption<bool>("TravelersRecall.CanTeleport.InBattleground", false);
        if (player->InBattleground() && !allowInBattleground)
        {
            handler->SendSysMessage("Traveler's Recall: cannot teleport from battlegrounds.");
            return false;
        }

        bool allowInDungeonOrRaid = sConfigMgr->GetOption<bool>("TravelersRecall.CanTeleport.InDungeonOrRaid", false);
        if (player->GetMap()->IsDungeon() && !allowInDungeonOrRaid)
        {
            handler->SendSysMessage("Traveler's Recall: cannot teleport from dungeons or raids.");
            return false;
        }

        return true;
    }

    std::string CanPlayerTeleportToFried(Player* player, Player* targetPlayer, ChatHandler* /*handler*/, uint32 /*locationId*/)
    {
        if (player->GetTransport() || targetPlayer->GetTransport())
        {
            return "Traveler's Recall: cannot teleport while you or target is on a transport.";
        }

        if ((player->IsInFlight() || targetPlayer->IsInFlight()))
        {
            return "Traveler's Recall: cannot teleport while you or target is flying.";
        }

        bool allowWhileStealthed = sConfigMgr->GetOption<bool>("TravelersRecall.CanTeleport.WhileStealthed", false);
        if ((player->HasStealthAura() || targetPlayer->HasStealthAura()) && !allowWhileStealthed)
        {
            return "Traveler's Recall: cannot teleport while you or target is stealthed.";
        }

        bool allowInCombat = sConfigMgr->GetOption<bool>("TravelersRecall.CanTeleport.InCombat", false);
        if ((player->IsInCombat() || targetPlayer->IsInCombat()) && !allowInCombat)
        {
            return "Traveler's Recall: cannot teleport while you or target is in combat.";
        }

        if ((player->isDead() || targetPlayer->isDead()))
        {
            return "Traveler's Recall: cannot teleport while you or target are dead.";
        }

        if ((player->InBattleground() || targetPlayer->InBattleground()))
        {
            return "Traveler's Recall: cannot teleport from or to battlegrounds.";
        }

        if ((player->GetMap()->IsDungeon() || targetPlayer->GetMap()->IsDungeon()))
        {
            return "Traveler's Recall: cannot teleport from or to dungeons or raids.";
        }

        if (!player->IsInSameGroupWith(targetPlayer))
        {
            return "Traveler's Recall: The target player is not in your group.";
        }

        return "OK";
    }
}

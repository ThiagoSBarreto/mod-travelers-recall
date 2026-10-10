#pragma once

#include "TravelersRecall.h"

namespace TravelersRecall
{
    bool HasUnlockedLocation(Player* player, uint32 locationId);
    uint32 GetCooldownRemaining(Player* player, uint32 locationId);
    bool CanPlayerTeleport(Player* player, ChatHandler* handler, uint32 locationId);
    std::string CanPlayerTeleportToFried(Player* player, Player* targetPlayer, ChatHandler* handler, uint32 locationId);
}

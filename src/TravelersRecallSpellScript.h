#pragma once

#include "TravelersRecall.h"

namespace TravelersRecall
{
    // Starts the Traveler's Recall cast bar. Returns true when the command is handled
    // (cast started, or refused with a client error); false when the caller should
    // teleport instantly instead (cast disabled or the configured spell can't be cast).
    bool StartRecallCast(Player* player, ChatHandler* handler, uint32 locationId, ObjectGuid targetGuid = ObjectGuid::Empty);
    bool IsRecallCasting(Player* player);
    void ClearRecallCast(Player* player);
}

void AddTravelersRecallSpellScripts();

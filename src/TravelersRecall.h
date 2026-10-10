#pragma once

#include "ScriptMgr.h"
#include "Player.h"
#include "Chat.h"
#include "Config.h"
#include "Log.h"
#include "ChatCommand.h"
#include "Tokenize.h"
#include "GameTime.h"
#include "DatabaseEnv.h"

#include <chrono>
#include <cstdint>
#include <string>

using namespace std::chrono;

namespace TravelersRecall
{
    void UnlockLocations(Player* player, std::string const& locations);
    std::string GetRaceTeleportConfig(Player* player);
    void AddTeleportToFriend(Player* player);

    void SendAddonMessage(Player* player, std::string const& message);
    void SendUnlockMessage(Player* player, uint32 locationId, std::string const& locationName, std::string const& icon);
    void SendRemoveMessage(Player* player, uint32 locationId);
}

void Addmod_travelers_recallScripts();

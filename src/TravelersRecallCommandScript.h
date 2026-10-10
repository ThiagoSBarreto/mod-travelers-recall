#pragma once

#include "TravelersRecall.h"

class TravelersRecallCommandScript : public CommandScript
{
public:
    TravelersRecallCommandScript();

    Acore::ChatCommands::ChatCommandTable GetCommands() const override;

    static bool SendListForPlayer(Player* player);
    static bool HandleListCommand(ChatHandler* handler, char const* args);
    static bool HandleTeleportCommand(ChatHandler* handler, char const* args);
    static bool HandleLearnCommand(ChatHandler* handler, char const* args);
    static bool HandleLearnAllCommand(ChatHandler* handler, char const* args);
    static bool HandleRemoveCommand(ChatHandler* handler, char const* args);
    static bool HandleRemoveAllCommand(ChatHandler* handler, char const* args);
    static bool HandleCooldownCommand(ChatHandler* handler, char const* args);
    static bool HandleCooldownListCommand(ChatHandler* handler, char const* args);
    static bool HandleCooldownResetCommand(ChatHandler* handler, char const* args);
    static bool HandleCooldownResetAllCommand(ChatHandler* handler, char const* args);

    static std::string BuildCooldownRemainingText(uint32 remainingSeconds);
    static bool HandleTeleportToFriend(Player *player, ChatHandler *handler, uint32 locationId, uint32 cooldown);
};

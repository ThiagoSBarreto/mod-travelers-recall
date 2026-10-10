#pragma once

#include "TravelersRecall.h"

class TravelersRecallPlayerScript : public PlayerScript
{
public:
    TravelersRecallPlayerScript();

    void OnPlayerUpdateArea(Player* player, uint32 oldArea, uint32 newArea) override;
    void OnPlayerDelete(ObjectGuid guid, uint32 account) override;
    void OnPlayerCreate(Player* player) override;
    void OnPlayerFirstLogin(Player* player) override;
    void OnPlayerLogin(Player* player) override;
};

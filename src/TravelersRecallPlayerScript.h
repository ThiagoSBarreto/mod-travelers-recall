#pragma once

#include "ScriptMgr.h"
#include "TravelersRecall.h"

class Player;

class TravelersRecallPlayerScript : public PlayerScript
{
public:
    TravelersRecallPlayerScript();

    void OnPlayerUpdateArea(Player* player, uint32 oldArea, uint32 newArea) override;
    void OnPlayerDelete(ObjectGuid guid, uint32 account) override;
    void OnPlayerCreate(Player* player) override;
    void OnPlayerLogin(Player* player) override;
    void OnPlayerLogout(Player* player) override;
    void OnPlayerFirstLogin(Player* player) override;
    
    static void CacheUnlock(Player* player, uint32 locationId);
    static void CacheRemove(Player* player, uint32 locationId);
    static void CacheRemoveAll(Player* player);
    static bool IsHeadlessClientSession(WorldSession const *session);
};
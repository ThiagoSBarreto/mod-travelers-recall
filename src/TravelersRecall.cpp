#include "TravelersRecall.h"
#include "TravelersRecallPlayerScript.h"
#include "TravelersRecallCommandScript.h"
#include "TravelersRecallSpellScript.h"

namespace TravelersRecall
{
    void SendAddonMessage(Player* player, std::string const& message)
    {
        ChatHandler(player->GetSession()).SendSysMessage(message);
    }

    void SendUnlockMessage(Player* player, uint32 locationId, std::string const& locationName, std::string const& icon)
    {
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

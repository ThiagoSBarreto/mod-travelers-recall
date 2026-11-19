#include "TravelersRecallSpellScript.h"
#include "TravelersRecallCommandScript.h"

#include "Spell.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"

#include <unordered_map>

namespace
{
    // "Translocate" from the Duskwither Spire orbs in Eversong Woods: a 3 second cast
    // with the Blood Elf translocation visual. Its own teleport is suppressed.
    constexpr uint32 RecallSpellId = 26566;

    // Stored in SpellValue::MaxAffectedTargets of casts started by this module, so the
    // spell script only takes over our own casts. Using the real orb keeps the spell's
    // own value and works as before.
    constexpr uint32 RecallCastMarker = 0x54524331;

    struct PendingRecall
    {
        uint32 locationId = 0;
        ObjectGuid targetGuid;
    };

    // Accessed from the world thread only (chat commands and spell updates).
    std::unordered_map<uint32, PendingRecall> PendingRecalls;

    bool IsRecallSpell(Spell* spell)
    {
        return spell && spell->GetSpellValue()->MaxAffectedTargets == RecallCastMarker;
    }
}

namespace TravelersRecall
{
    bool StartRecallCast(Player* player, ChatHandler* handler, uint32 locationId, ObjectGuid targetGuid)
    {
        if (!sConfigMgr->GetOption<bool>("TravelersRecall.CastTime.Enable", false))
        {
            return false;
        }

        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(RecallSpellId);

        // Instant spells would finish inside CastSpell, so only use the spell with a cast bar.
        if (!spellInfo || !spellInfo->CalcCastTime())
        {
            return false;
        }

        if (IsRecallCasting(player))
        {
            handler->SendSysMessage("Traveler's Recall: you are already casting a teleport.");
            return true;
        }

        // Like a Hearthstone, the cast dismounts the player instead of failing.
        if (player->IsMounted() && !player->IsInFlight())
        {
            player->Dismount();
            player->RemoveAurasByType(SPELL_AURA_MOUNTED);
        }

        uint32 guid = player->GetGUID().GetCounter();
        PendingRecalls[guid] = { locationId, targetGuid };

        SpellCastTargets targets;
        targets.SetUnitTarget(player);

        CustomSpellValues values;
        values.AddSpellMod(SPELLVALUE_MAX_TARGETS, static_cast<int32>(RecallCastMarker));

        SpellCastResult result = player->CastSpell(targets, spellInfo, &values, TRIGGERED_NONE);
        if (result == SPELL_CAST_OK)
        {
            return true;
        }

        PendingRecalls.erase(guid);

        switch (result)
        {
        // The client already shows the reason; don't fall back to an instant teleport.
        case SPELL_FAILED_MOVING:
        case SPELL_FAILED_NOT_READY:
        case SPELL_FAILED_SPELL_IN_PROGRESS:
        case SPELL_FAILED_INTERRUPTED:
            return true;
        default:
            return false;
        }
    }

    bool IsRecallCasting(Player* player)
    {
        return IsRecallSpell(player->GetCurrentSpell(CURRENT_GENERIC_SPELL));
    }

    void ClearRecallCast(Player* player)
    {
        PendingRecalls.erase(player->GetGUID().GetCounter());
    }
}

// Bound to the Translocate spell in spell_script_names. For our casts it suppresses
// the spell's own effects (the orb's teleport) and runs the Traveler's
// Recall teleport when the cast bar completes. Interrupting the cast (moving, jumping,
// Escape) never reaches AfterCast, so nothing is teleported and no cooldown starts.
class spell_travelers_recall_cast : public SpellScript
{
    PrepareSpellScript(spell_travelers_recall_cast);

    void PreventOrbEffect(SpellEffIndex effIndex)
    {
        if (IsRecallSpell(GetSpell()))
        {
            PreventHitDefaultEffect(effIndex);
        }
    }

    void HandleAfterCast()
    {
        if (!IsRecallSpell(GetSpell()))
        {
            return;
        }

        Player* player = GetCaster() ? GetCaster()->ToPlayer() : nullptr;
        if (!player)
        {
            return;
        }

        auto itr = PendingRecalls.find(player->GetGUID().GetCounter());
        if (itr == PendingRecalls.end())
        {
            return;
        }

        PendingRecall pending = itr->second;
        PendingRecalls.erase(itr);

        // Runs every check again (combat, cooldown, target...) before teleporting.
        ChatHandler handler(player->GetSession());
        TravelersRecallCommandScript::TeleportToLocation(player, &handler, pending.locationId, false, pending.targetGuid);
    }

    void Register() override
    {
        OnEffectLaunch += SpellEffectFn(spell_travelers_recall_cast::PreventOrbEffect, EFFECT_ALL, SPELL_EFFECT_ANY);
        OnEffectLaunchTarget += SpellEffectFn(spell_travelers_recall_cast::PreventOrbEffect, EFFECT_ALL, SPELL_EFFECT_ANY);
        OnEffectHit += SpellEffectFn(spell_travelers_recall_cast::PreventOrbEffect, EFFECT_ALL, SPELL_EFFECT_ANY);
        OnEffectHitTarget += SpellEffectFn(spell_travelers_recall_cast::PreventOrbEffect, EFFECT_ALL, SPELL_EFFECT_ANY);
        AfterCast += SpellCastFn(spell_travelers_recall_cast::HandleAfterCast);
    }
};

void AddTravelersRecallSpellScripts()
{
    RegisterSpellScript(spell_travelers_recall_cast);
}

#include "Chat.h"
#include "Config.h"
#include "GameObject.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellInfo.h"

#include <array>
#include <chrono>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace EmeraldDreamway
{
    constexpr uint32 MAP_EMERALD_DREAM = 169;
    constexpr uint32 GRAVEYARD_VERDANT_FIELDS = 990101;
    constexpr float GRAVEYARD_VERDANT_FIELDS_ORIENTATION = 4.9060855f;

    enum GhostFlightSpells : uint32
    {
        SPELL_SWIFT_SPECTRAL_GRYPHON = 55164,
        SPELL_SWIFT_SPECTRAL_WIND_RIDER = 55173
    };

    bool IsSpectralGhostFlightSpell(SpellInfo const* spellInfo)
    {
        return spellInfo &&
            (spellInfo->Id == SPELL_SWIFT_SPECTRAL_GRYPHON ||
             spellInfo->Id == SPELL_SWIFT_SPECTRAL_WIND_RIDER);
    }

    bool HasSpectralGhostFlight(Player const* player)
    {
        return player &&
            (player->HasAura(SPELL_SWIFT_SPECTRAL_GRYPHON) ||
             player->HasAura(SPELL_SWIFT_SPECTRAL_WIND_RIDER) ||
             player->HasIncreaseMountedFlightSpeedAura() ||
             player->HasFlyAura());
    }

    void RemoveSpectralGhostFlight(Player* player)
    {
        if (!player || player->GetMapId() != MAP_EMERALD_DREAM || player->IsAlive())
            return;

        player->RemoveAurasDueToSpell(SPELL_SWIFT_SPECTRAL_GRYPHON);
        player->RemoveAurasDueToSpell(SPELL_SWIFT_SPECTRAL_WIND_RIDER);

        if (player->IsMounted())
            player->Dismount();

        player->SetCanFly(false);
    }

    enum GameObjectEntries : uint32
    {
        // Duskwood
        GO_TWILIGHT_GROVE_TO_DREAMWAY = 990001,
        GO_DREAMWAY_TO_TWILIGHT_GROVE = 990002,

        // Ashenvale
        GO_BOUGH_SHADOW_TO_DREAMWAY = 990003,
        GO_DREAMWAY_TO_BOUGH_SHADOW = 990004,

        // Feralas
        GO_DREAM_BOUGH_TO_DREAMWAY = 990005,
        GO_DREAMWAY_TO_DREAM_BOUGH = 990006,

        // Hinterlands
        GO_SERADANE_TO_DREAMWAY = 990007,
        GO_DREAMWAY_TO_SERADANE = 990008
    };


    struct TeleportDestination
    {
        uint32 MapId;
        float X;
        float Y;
        float Z;
        float Orientation;
    };

    struct DreamwayRoute
    {
        uint32 GameObjectEntry;
        uint32 SourceMapId;
        char const* EnabledConfigKey;
        TeleportDestination Destination;
    };

    constexpr std::array<DreamwayRoute, 8> ROUTES =
    {{
        // Twilight Grove, Duskwood <-> Verdant Fields
        {
            GO_TWILIGHT_GROVE_TO_DREAMWAY,
            0,
            "EmeraldDreamway.Route.TwilightGrove.Enable",
            { 169, -2048.6118f, -960.6592f, 135.36461f, 1.6953764f }
        },
        {
            GO_DREAMWAY_TO_TWILIGHT_GROVE,
            169,
            "EmeraldDreamway.Route.TwilightGrove.Enable",
            { 0, -10371.299f, -421.52963f, 63.62110f, 3.1898613f }
        },

        // Bough Shadow, Ashenvale <-> Verdant Fields
        {
            GO_BOUGH_SHADOW_TO_DREAMWAY,
            1,
            "EmeraldDreamway.Route.BoughShadow.Enable",
            { 169, -1996.7683f, -873.16077f, 129.8715f, 3.7413394f }
        },
        {
            GO_DREAMWAY_TO_BOUGH_SHADOW,
            169,
            "EmeraldDreamway.Route.BoughShadow.Enable",
            { 1, 3311.303f, -3734.423f, 173.45723f, 2.948247f }
        },

        // Dream Bough, Feralas <-> Verdant Fields
        {
            GO_DREAM_BOUGH_TO_DREAMWAY,
            1,
            "EmeraldDreamway.Route.DreamBough.Enable",
            { 169, -2129.5247f, -1007.08514f, 132.26376f, 0.5958505f }
        },
        {
            GO_DREAMWAY_TO_DREAM_BOUGH,
            169,
            "EmeraldDreamway.Route.DreamBough.Enable",
            { 1, -2864.227f, 1879.3861f, 52.646618f, 2.7819788f }
        },

        // Seradane, Hinterlands <-> Verdant Fields
        {
            GO_SERADANE_TO_DREAMWAY,
            0,
            "EmeraldDreamway.Route.Seradane.Enable",
            { 169, -2131.3113f, -896.9243f, 135.37054f, 5.7244782f }
        },
        {
            GO_DREAMWAY_TO_SERADANE,
            169,
            "EmeraldDreamway.Route.Seradane.Enable",
            { 0, 874.5052f, -3972.7332f, 145.82391f, 3.4519851f }
        }
    }};

    using Clock = std::chrono::steady_clock;

    struct PendingTeleport
    {
        uint32 GameObjectEntry;
        uint32 SourceMapId;
        float StartX;
        float StartY;
        float StartZ;
        Clock::time_point ExpiresAt;
    };

    constexpr float MAX_CAST_MOVEMENT_DISTANCE = 0.5f;
    constexpr float MAX_CAST_MOVEMENT_DISTANCE_SQ =
        MAX_CAST_MOVEMENT_DISTANCE * MAX_CAST_MOVEMENT_DISTANCE;

    std::unordered_map<uint32, PendingTeleport> PendingTeleports;
    std::mutex PendingTeleportsMutex;

    DreamwayRoute const* FindRoute(uint32 gameObjectEntry)
    {
        for (DreamwayRoute const& route : ROUTES)
        {
            if (route.GameObjectEntry == gameObjectEntry)
                return &route;
        }

        return nullptr;
    }

    uint32 GetPlayerKey(Player const* player)
    {
        return player->GetGUID().GetCounter();
    }

    void SendPlayerMessage(Player* player, std::string_view message)
    {
        if (player && player->GetSession())
            ChatHandler(player->GetSession()).SendSysMessage(message);
    }

    bool ValidatePlayerForRoute(Player* player, DreamwayRoute const& route)
    {
        if (!sConfigMgr->GetOption<bool>("EmeraldDreamway.Enable", true) ||
            !sConfigMgr->GetOption<bool>("EmeraldDreamway.Pedestals.Enable", true))
        {
            SendPlayerMessage(player, "The Emerald Dreamway is currently unavailable.");
            return false;
        }

        if (!sConfigMgr->GetOption<bool>(route.EnabledConfigKey, true))
        {
            SendPlayerMessage(player, "This Dreamway route is currently unavailable.");
            return false;
        }

        if (player->GetMapId() != route.SourceMapId)
        {
            SendPlayerMessage(player, "This pedestal is not anchored to the correct part of the Dreamway.");
            return false;
        }

        uint32 minimumLevel =
            sConfigMgr->GetOption<uint32>("EmeraldDreamway.MinimumLevel", 1);

        if (player->GetLevel() < minimumLevel)
        {
            SendPlayerMessage(
                player,
                "You must be at least level " + std::to_string(minimumLevel) +
                    " to use the Emerald Dreamway.");
            return false;
        }

        if (!sConfigMgr->GetOption<bool>("EmeraldDreamway.AllowInCombat", false) &&
            player->IsInCombat())
        {
            SendPlayerMessage(player, "You cannot use the Emerald Dreamway while in combat.");
            return false;
        }

        if (!sConfigMgr->GetOption<bool>("EmeraldDreamway.AllowDead", false) &&
            !player->IsAlive())
        {
            SendPlayerMessage(player, "You cannot use the Emerald Dreamway while dead.");
            return false;
        }

        if (!sConfigMgr->GetOption<bool>("EmeraldDreamway.AllowInVehicle", false) &&
            player->GetVehicle())
        {
            SendPlayerMessage(player, "You cannot use the Emerald Dreamway while in a vehicle.");
            return false;
        }

        return true;
    }

    bool TeleportPlayer(Player* player, DreamwayRoute const& route)
    {
        TeleportDestination const& destination = route.Destination;

        if (!player->TeleportTo(
                destination.MapId,
                destination.X,
                destination.Y,
                destination.Z,
                destination.Orientation))
        {
            SendPlayerMessage(player, "The Emerald Dreamway could not be reached.");
            return false;
        }

        return true;
    }

    void StorePendingTeleport(Player* player, DreamwayRoute const& route)
    {
        PendingTeleport pending
        {
            route.GameObjectEntry,
            route.SourceMapId,
            player->GetPositionX(),
            player->GetPositionY(),
            player->GetPositionZ(),
            Clock::now() + std::chrono::seconds(5)
        };

        std::lock_guard<std::mutex> lock(PendingTeleportsMutex);
        PendingTeleports[GetPlayerKey(player)] = pending;
    }

    void RemovePendingTeleport(Player const* player)
    {
        if (!player)
            return;

        std::lock_guard<std::mutex> lock(PendingTeleportsMutex);
        PendingTeleports.erase(GetPlayerKey(player));
    }

    std::optional<PendingTeleport> TakePendingTeleport(Player const* player)
    {
        std::lock_guard<std::mutex> lock(PendingTeleportsMutex);

        auto itr = PendingTeleports.find(GetPlayerKey(player));
        if (itr == PendingTeleports.end())
            return std::nullopt;

        PendingTeleport pending = itr->second;
        PendingTeleports.erase(itr);

        if (Clock::now() > pending.ExpiresAt)
            return std::nullopt;

        return pending;
    }

    void CompletePendingTeleport(Player* player)
    {
        std::optional<PendingTeleport> pending = TakePendingTeleport(player);
        if (!pending)
            return;

        DreamwayRoute const* route = FindRoute(pending->GameObjectEntry);
        if (!route)
            return;

        if (!ValidatePlayerForRoute(player, *route))
            return;

        if (player->GetMapId() != pending->SourceMapId)
            return;

        float deltaX = player->GetPositionX() - pending->StartX;
        float deltaY = player->GetPositionY() - pending->StartY;
        float deltaZ = player->GetPositionZ() - pending->StartZ;
        float distanceSq = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;

        if (distanceSq > MAX_CAST_MOVEMENT_DISTANCE_SQ)
        {
            SendPlayerMessage(player, "The Dreamway activation was interrupted.");
            return;
        }

        TeleportPlayer(player, *route);
        
    }
}

class go_emerald_dreamway_pedestal : public GameObjectScript
{
public:
    go_emerald_dreamway_pedestal()
        : GameObjectScript("go_emerald_dreamway_pedestal")
    {
    }

    bool OnGossipHello(Player* player, GameObject* gameObject) override
    {
        using namespace EmeraldDreamway;

        if (!player || !gameObject)
            return false;

        DreamwayRoute const* route = FindRoute(gameObject->GetEntry());
        if (!route)
            return false;

        RemovePendingTeleport(player);

        if (!ValidatePlayerForRoute(player, *route))
            return true;

        if (!sConfigMgr->GetOption<bool>("EmeraldDreamway.Cast.Enable", true))
        {
            TeleportPlayer(player, *route);
            return true;
        }

        if (player->IsNonMeleeSpellCast(false))
        {
            SendPlayerMessage(player, "You are already casting another spell.");
            return true;
        }

        StorePendingTeleport(player, *route);

        // Continue with AzerothCore's normal Goober activation.
        // The GameObject template casts spell 60957 from Data10.  
        return false;
    }
};

class spell_emerald_dreamway_activation : public SpellScript
{
    PrepareSpellScript(spell_emerald_dreamway_activation);

    void HandleAfterCast()
    {
        if (Player* player = GetCaster()->ToPlayer())
            EmeraldDreamway::CompletePendingTeleport(player);
    }

    void Register() override
    {
        AfterCast += SpellCastFn(spell_emerald_dreamway_activation::HandleAfterCast);
    }
};

class emerald_dreamway_player_cleanup : public PlayerScript
{
public:
    emerald_dreamway_player_cleanup()
        : PlayerScript("emerald_dreamway_player_cleanup")
    {
    }

    void OnPlayerLogin(Player* player) override
    {
        if (EmeraldDreamway::HasSpectralGhostFlight(player))
            EmeraldDreamway::RemoveSpectralGhostFlight(player);
    }

    void OnPlayerMapChanged(Player* player) override
    {
        if (EmeraldDreamway::HasSpectralGhostFlight(player))
            EmeraldDreamway::RemoveSpectralGhostFlight(player);
    }

    void OnPlayerReleasedGhost(Player* player) override
    {
        EmeraldDreamway::RemoveSpectralGhostFlight(player);
    }

    void OnPlayerUpdate(Player* player, uint32 /*diff*/) override
    {
        // The spectral ghost mount can be applied during the zone update that
        // follows the graveyard teleport, after OnPlayerReleasedGhost runs.
        if (EmeraldDreamway::HasSpectralGhostFlight(player))
            EmeraldDreamway::RemoveSpectralGhostFlight(player);
    }

    bool OnPlayerCanFlyInZone(
        Player* player,
        uint32 mapId,
        uint32 /*zoneId*/,
        SpellInfo const* bySpell) override
    {
        if (player && !player->IsAlive() &&
            mapId == EmeraldDreamway::MAP_EMERALD_DREAM &&
            EmeraldDreamway::IsSpectralGhostFlightSpell(bySpell))
        {
            return false;
        }

        return true;
    }

    void OnPlayerLogout(Player* player) override
    {
        EmeraldDreamway::RemovePendingTeleport(player);
    }

    void OnPlayerBeforeChooseGraveyard(
        Player* player,
        TeamId /*teamId*/,
        bool nearCorpse,
        uint32& graveyardOverride) override
    {
        if (!player)
            return;

        uint32 mapId = player->GetMapId();

        if (nearCorpse)
            mapId = player->GetCorpseLocation().GetMapId();

        if (mapId == EmeraldDreamway::MAP_EMERALD_DREAM)
        {
            // game_graveyard stores no orientation, and RepopAtGraveyard uses
            // the player's current orientation for the graveyard teleport.
            player->SetOrientation(EmeraldDreamway::GRAVEYARD_VERDANT_FIELDS_ORIENTATION);
            graveyardOverride = EmeraldDreamway::GRAVEYARD_VERDANT_FIELDS;
        }
    }
};

void AddEmeraldDreamwayScripts()
{
    new go_emerald_dreamway_pedestal();
    new emerald_dreamway_player_cleanup();

    RegisterSpellScript(spell_emerald_dreamway_activation);
}

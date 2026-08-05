#include "Chat.h"
#include "Config.h"
#include "GameObject.h"
#include "Player.h"
#include "ScriptMgr.h"

#include <array>
#include <string>
#include <string_view>

namespace EmeraldDreamway
{
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
            { 169, -2060.9365f, -942.6356f, 131.84442f, 3.6274557f }
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
            { 169, -1999.9692f, -887.1238f, 128.27328f, 3.947896f }
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
            { 169, -2124.1707f, -985.322f, 130.74112f, 0.87699795f }
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
            { 169, -2125.8394f, -909.9735f, 135.20438f, 5.7817802f }
        },
        {
            GO_DREAMWAY_TO_SERADANE,
            169,
            "EmeraldDreamway.Route.Seradane.Enable",
            { 0, 874.5052f, -3972.7332f, 145.82391f, 3.4519851f }
        }
    }};

    DreamwayRoute const* FindRoute(uint32 gameObjectEntry)
    {
        for (DreamwayRoute const& route : ROUTES)
        {
            if (route.GameObjectEntry == gameObjectEntry)
                return &route;
        }

        return nullptr;
    }

    void SendPlayerMessage(Player* player, std::string_view message)
    {
        if (player && player->GetSession())
            ChatHandler(player->GetSession()).SendSysMessage(message);
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

        if (!sConfigMgr->GetOption<bool>("EmeraldDreamway.Enable", true) ||
            !sConfigMgr->GetOption<bool>("EmeraldDreamway.Pedestals.Enable", true))
        {
            SendPlayerMessage(player, "The Emerald Dreamway is currently unavailable.");
            return true;
        }

        if (!sConfigMgr->GetOption<bool>(route->EnabledConfigKey, true))
        {
            SendPlayerMessage(player, "This Dreamway route is currently unavailable.");
            return true;
        }

        if (player->GetMapId() != route->SourceMapId)
        {
            SendPlayerMessage(player, "This pedestal is not anchored to the correct part of the Dreamway.");
            return true;
        }

        uint32 minimumLevel = sConfigMgr->GetOption<uint32>("EmeraldDreamway.MinimumLevel", 1);
        if (player->GetLevel() < minimumLevel)
        {
            SendPlayerMessage(
                player,
                "You must be at least level " + std::to_string(minimumLevel) +
                    " to use the Emerald Dreamway.");
            return true;
        }

        if (!sConfigMgr->GetOption<bool>("EmeraldDreamway.AllowInCombat", false) &&
            player->IsInCombat())
        {
            SendPlayerMessage(player, "You cannot use the Emerald Dreamway while in combat.");
            return true;
        }

        if (!sConfigMgr->GetOption<bool>("EmeraldDreamway.AllowDead", false) &&
            !player->IsAlive())
        {
            SendPlayerMessage(player, "You cannot use the Emerald Dreamway while dead.");
            return true;
        }

        if (!sConfigMgr->GetOption<bool>("EmeraldDreamway.AllowInVehicle", false) &&
            player->GetVehicle())
        {
            SendPlayerMessage(player, "You cannot use the Emerald Dreamway while in a vehicle.");
            return true;
        }

        TeleportDestination const& destination = route->Destination;

        if (!player->TeleportTo(
                destination.MapId,
                destination.X,
                destination.Y,
                destination.Z,
                destination.Orientation))
        {
            SendPlayerMessage(player, "The Emerald Dreamway could not be reached.");
        }

        return true;
    }
};

void AddEmeraldDreamwayScripts()
{
    new go_emerald_dreamway_pedestal();
}

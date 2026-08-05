#include "GameObject.h"
#include "Player.h"
#include "ScriptMgr.h"

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

    // Duskwood -> Verdant Fields
    constexpr TeleportDestination VERDANT_FIELDS_DUSKWOOD_ARRIVAL
    {
        169,
        -2060.9365f,
        -942.6356f,
        131.84442f,
        3.6274557f
    };

    // Verdant Fields -> Duskwood
    constexpr TeleportDestination TWILIGHT_GROVE_ARRIVAL
    {
        0,
        -10371.299f,
        -421.52963f,
        63.62110f,
        3.1898613f
    };

    // Ashenvale -> Verdant Fields
    constexpr TeleportDestination VERDANT_FIELDS_ASHENVALE_ARRIVAL
    {
        169,
        -1999.9692f,
        -887.1238f,
        128.27328f,
        3.947896f
    };

    // Verdant Fields -> Ashenvale
    constexpr TeleportDestination BOUGH_SHADOW_ARRIVAL
    {
        1,
        3311.303f,
        -3734.423f,
        173.45723f,
        2.948247f
    };

    // Feralas -> Verdant Fields
    constexpr TeleportDestination VERDANT_FIELDS_FERALAS_ARRIVAL
    {
        169,
        -2124.1707f,
        -985.322f,
        130.74112f,
        0.87699795f
    };

    // Verdant Fields -> Feralas
    constexpr TeleportDestination DREAM_BOUGH_ARRIVAL
    {
        1,
        -2864.227f,
        1879.3861f,
        52.646618f,
        2.7819788f
    };

    // Hinterlands -> Verdant Fields
    constexpr TeleportDestination VERDANT_FIELDS_HINTERLANDS_ARRIVAL
    {
        169,
        -2125.8394f,
        -909.9735f,
        135.20438f,
        5.7817802f
    };

    // Verdant Fields -> Hinterlands
    constexpr TeleportDestination SERADANE_ARRIVAL
    {
        0,
        874.5052f,
        -3972.7332f,
        145.82391f,
        3.4519851f
    };
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

        TeleportDestination const* destination = nullptr;

        switch (gameObject->GetEntry())
        {
            case GO_TWILIGHT_GROVE_TO_DREAMWAY:
                destination = &VERDANT_FIELDS_DUSKWOOD_ARRIVAL;
                break;

            case GO_DREAMWAY_TO_TWILIGHT_GROVE:
                destination = &TWILIGHT_GROVE_ARRIVAL;
                break;

            case GO_BOUGH_SHADOW_TO_DREAMWAY:
                destination = &VERDANT_FIELDS_ASHENVALE_ARRIVAL;
                break;

            case GO_DREAMWAY_TO_BOUGH_SHADOW:
                destination = &BOUGH_SHADOW_ARRIVAL;
                break;

            case GO_DREAM_BOUGH_TO_DREAMWAY:
                destination = &VERDANT_FIELDS_FERALAS_ARRIVAL;
                break;

            case GO_DREAMWAY_TO_DREAM_BOUGH:
                destination = &DREAM_BOUGH_ARRIVAL;
                break;

            case GO_SERADANE_TO_DREAMWAY:
                destination = &VERDANT_FIELDS_HINTERLANDS_ARRIVAL;
                break;

            case GO_DREAMWAY_TO_SERADANE:
                destination = &SERADANE_ARRIVAL;
                break;

            default:
                return false;
        }

        player->TeleportTo(
            destination->MapId,
            destination->X,
            destination->Y,
            destination->Z,
            destination->Orientation);

        return true;
    }
};

void AddEmeraldDreamwayScripts()
{
    new go_emerald_dreamway_pedestal();
}

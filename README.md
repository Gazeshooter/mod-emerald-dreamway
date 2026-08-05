# Emerald Dreamway Module

`mod-emerald-dreamway` is an AzerothCore module for World of Warcraft 3.3.5a that connects the four classic Great Tree portal locations in Kalimdor and the Eastern Kingdoms through a central hub in the Verdant Fields.

The module is inspired by the Emerald Dreamway introduced in Legion. It repurposes the unused Verdant Fields area and populates it with clickable GameObjects, requiring no client-side MPQ or DBC modifications.

## TO DO
  * NPC detailing
  * NPCs (innkeeper, vendor, reagents) toggle config
  * Camp decoration

## Features

- Four Dreamway routes:
  - Twilight Grove, Duskwood
  - Bough Shadow, Ashenvale
  - Dream Bough, Feralas
  - Seradane, Hinterlands
- Un-instances Emerald Dream zone:
  - Changes map 169 from a raid to a common world map through a `map_dbc` override
  - Allows players to occupy the same Emerald Dream map and see each other
- Graveyard with spirit healer
- Master on/off switch.
- Individual on/off switches for each route.
- Optional two-second nature-themed activation cast.
- Configurable restrictions for:
  - Combat
  - Dead players
  - Vehicles
  - Player level
- Player-specific error and availability messages.
- Automatic installation of database records.

<img width="3116" height="796" alt="image" src="https://github.com/user-attachments/assets/940829eb-df95-47ed-ae89-634d6d7194e6" />

## Installation

1. Clone the repository into the AzerothCore `modules` directory.
2. Reconfigure and rebuild AzerothCore.
3. Start `worldserver`

## Configuration

| Setting | Default | Description |
| --- | --- | --- |
| `EmeraldDreamway.Enable` | `1` | Enables the complete Dreamway system |
| `EmeraldDreamway.Pedestals.Enable` | `1` | Enables clickable teleport pedestals |
| `EmeraldDreamway.Cast.Enable` | `1` | Enables the two-second activation cast |
| `EmeraldDreamway.MinimumLevel` | `1` | Minimum level required |
| `EmeraldDreamway.AllowInCombat` | `0` | Allows use during combat |
| `EmeraldDreamway.AllowDead` | `0` | Allows dead players and ghosts |
| `EmeraldDreamway.AllowInVehicle` | `0` | Allows use while in a vehicle |
| `EmeraldDreamway.Route.TwilightGrove.Enable` | `1` | Enables the Duskwood route |
| `EmeraldDreamway.Route.BoughShadow.Enable` | `1` | Enables the Ashenvale route |
| `EmeraldDreamway.Route.DreamBough.Enable` | `1` | Enables the Feralas route |
| `EmeraldDreamway.Route.Seradane.Enable` | `1` | Enables the Hinterlands route |

## Activation cast

When `EmeraldDreamway.Cast.Enable` is enabled, interacting with a pedestal triggers a two-second nature cast before teleporting. The cast uses the stock client spell `Cosmetic Nature Cast`. Creating a custom spell would require a DBC edit, which is outside the scope of this module.

Moving more than 0.5 yards during the cast prevents the teleport. Player eligibility is checked both at the start and end of the cast.

## License

This project is licensed under the GNU General Public License version 2 or later. See [LICENSE](LICENSE).

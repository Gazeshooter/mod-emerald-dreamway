# Emerald Dreamway Module
`mod-emerald-dreamway` is an AzerothCore module for World of Warcraft 3.3.5a that connects the four classic Great Tree portal locations in Kalimdor and the Eastern Kingdoms through a central hub in the Verdant Fields.

The module is inspired by the Emerald Dreamway introduced in Legion. It repurposes the unused Verdant Fields area and populates it with clickable GameObjects, requiring no client-side MPQ or DBC modifications.

Features include
- Four Dreamway Routes
  - Twilight Grove, Duskwood
  - Bough Shadow, Ashenvale
  - Dream Bough, Feralas
  - Seradane, Hinterlands
- Master on/off switch
- Individual on/off switches for each route
- Configurable minimum character level
- Configurable restrictions for:
  - Combat
  - Dead players
  - Vehicles
- Player-specific error and availability messages
- Automatic installation of database records

# Installation
1. Clone repo to your AzerothCore modules directory
2. Rebuild and restart `worldserver`
The module's SQL files will automatically add the required GameObject templates and spawns to the world database

# Configuration
| Setting                            | Default | Description                          |
| ---------------------------------- | ------: | ------------------------------------ |
| `EmeraldDreamway.Enable`           |     `1` | Enables the complete Dreamway system |
| `EmeraldDreamway.Pedestals.Enable` |     `1` | Enables clickable teleport pedestals |
| `EmeraldDreamway.MinimumLevel`     |     `1` | Minimum level required               |
| `EmeraldDreamway.AllowInCombat`    |     `0` | Allows use during combat             |
| `EmeraldDreamway.AllowDead`        |     `0` | Allows dead players and ghosts       |
| `EmeraldDreamway.AllowInVehicle`   |     `0` | Allows use while in a vehicle        |

Each route can also be enabled or disabled independently:

```ini
EmeraldDreamway.Route.TwilightGrove.Enable = 1
EmeraldDreamway.Route.BoughShadow.Enable = 1
EmeraldDreamway.Route.DreamBough.Enable = 1
EmeraldDreamway.Route.Seradane.Enable = 1
```

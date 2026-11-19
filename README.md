# Traveler's Recall Module

Progressive fast-travel system for AzerothCore.

![World of Warcraft Client](https://img.shields.io/badge/World%20of%20Warcraft%20Client-3.3.5a-blue)

---

## November 19, 2026 - Version 1.4.0 Released

63 new locations and fixes for several broken ones, after a full check of every location against AzerothCore's `game_tele` positions, AreaTable area IDs and flight master spawns.

* Added 49 quest hubs from [Tntdruid's fork](https://github.com/Tntdruid/mod-travelers-recall/commit/b04b4ac93d836646088bf13941a63d3ac0eb0a58), including Hammerfall, Stonard, Grom'gol Base Camp, Theramore Isle and all of Horde Northrend
* Added 14 flight points that had no location, such as The Bulwark, Shatter Point, The Argent Vanguard and Death's Rise
* Fixed Warsong Hold and The Exodar, which teleported to the wrong place, and Wildhammer Stronghold, Thrallmar, Mirage Raceway and two wrong zone icons

> [!NOTE]
> Apply the database updates after updating. Update the [client addon](https://github.com/EvilTactician/TravelersRecall) to version 1.3.0 so the new locations are grouped under their zone and region.

---

## Description

Traveler's Recall is a progressive teleport system for AzerothCore.

Players unlock travel locations by discovering cities and important places across the world. Once a destination is unlocked, it becomes available through the Traveler's Recall client addon.

---

## Images

<img width="49%" alt="image" src="https://github.com/user-attachments/assets/08c68f98-80b7-4360-9f38-a936e4f60326" />
<img width="49%" alt="image" src="https://github.com/user-attachments/assets/819b89fd-5ab5-4777-9ce1-fa57c4cb5042" />
<details>
<summary>View more screenshots</summary>
<img width="49%" alt="image" src="https://github.com/user-attachments/assets/855c04bd-0f93-4b1a-81f1-de1d8c67e403" />
<img width="49%" alt="image" src="https://github.com/user-attachments/assets/86e348e4-43a4-46f8-a61b-bd9d9b786f15" />

<img width="365" height="124" alt="image" src="https://github.com/user-attachments/assets/ce9a2e54-49b0-42f1-925c-bc1e58f583ed" />
<img width="524" height="149" alt="image" src="https://github.com/user-attachments/assets/091b803a-129f-46bc-a51a-764ba5aceac1" />
</details>

---

## Features

* Unlock teleport locations by discovering cities and important places
* Custom teleport window integration
* Global or individual teleport cooldowns
* Faction restrictions
* Player state protections
* Custom unlockable locations
* Level requirement support
* Initial unlocked teleports
* Teleport to a Friend, with its own configurable cooldown
* Optional 3-second cast before teleporting, cancellable without starting the cooldown
* GM Commands
* Optional party bot teleport support (enable `TravelersRecall.TeleportPartyBots` in the configuration)
* Optional Headless-client teleport support (enable `TravelersRecall.TeleportHeadlessClient` in the configuration)

---

## Required Addon

This module requires the separate client addon: [**TravelersRecall — Client Addon**](https://github.com/EvilTactician/TravelersRecall).

The addon provides the user interface used to browse and select unlocked teleport locations.

The addon window can be opened using:

```text
/tr
```

or by clicking the minimap button.

---

## 1. Installation

### 1.1. Clone the Module

Clone this repository into the `modules` directory of your AzerothCore source tree.

Example:

```bash
cd /path/to/azerothcore/modules
git clone https://github.com/EvilTactician/mod-travelers-recall.git
```

### 1.2. Rebuild AzerothCore

> After cloning the module, rebuild AzerothCore.

---

### 1.3. Apply the Database Updates

Apply the SQL files to their corresponding AzerothCore databases:

| Directory         | Database           |
| ----------------- | ------------------ |
| `data/world`      | `acore_world`      |
| `data/characters` | `acore_characters` |

> Depending on your AzerothCore version and database updater configuration, these updates may be applied automatically when `worldserver` starts.

---

### 1.4. Install the Module Configuration

The repository includes a default configuration template:

```text
mod-travelers-recall.conf.dist
```

Copy it into your AzerothCore `modules configuration directory` and rename it to:

```text
mod-travelers-recall.conf
```

Example:

```bash
cp /path/to/azerothcore/modules/mod-travelers-recall.conf.dist \
   /path/to/azerothcore/env/dist/etc/modules/mod-travelers-recall.conf
```

Your resulting structure should look similar to:

```text
azerothcore/
└── env/
    └── dist/
        └── etc/
            └── modules/
                └── mod-travelers-recall.conf
```

> If your AzerothCore version automatically copies module configuration files during the build or installation process, verify whether the file has already been placed in the `env/dist/etc/modules` directory before copying it manually.

---

### 1.5. Install the Client Addon

Download the client addon from: [**TravelersRecall — Client Addon**](https://github.com/EvilTactician/TravelersRecall).

Install it into your World of Warcraft client directory.

The final directory structure should look similar to:

```text
World of Warcraft/
└── Interface/
    └── AddOns/
        └── TravelersRecall/
```

This module was created for WoW version **3.3.5a**.

> After installing the addon, restart the game client and verify that it is enabled on the character-selection screen.

---

## 2. Customization

### 2.1. Individual Cooldowns

Teleport locations are stored in:

```text
acore_world.custom_travelers_recall_locations
```

The `cooldown` column controls the cooldown for each individual location.

* `0`: Uses the global cooldown configured by the module
    * If `TravelersRecall.UseDefaultCooldown` is set to `1` in the configuration file, all locations will have a default cooldown configured by `TravelersRecall.DefaultCooldown`.
* Greater than `0`: Uses an individual cooldown value in seconds

```text
3600  = 1 hour
7200  = 2 hours
86400 = 24 hours
```

Example:

```sql
UPDATE acore_world.custom_travelers_recall_locations
SET cooldown = 7200
WHERE name = 'Brill';
```

> This configures Brill to use a two-hour cooldown.

---

### 2.2. Level Requirement

The `required_level` column controls the required level for each individual location.

* `0`: Location has no level requirement
* Greater than `0`:
    * If `TravelersRecall.UseRequiredLevel` is set to `1` in the configuration file, players must be at least the specified level to unlock that location.

Example:

```sql
UPDATE acore_world.custom_travelers_recall_locations
SET required_level = 10
WHERE area_id = 159;
```

> This configures Brill to require at least level 10 to unlock the teleport location.

---

### 2.3. Faction Restriction

The `faction` column controls the faction restriction for that location.

* `0`: Location is `Neutral territory`, both factions can unlock it.
* `1`: Location is `Alliance territory`, only Alliance players can unlock it.
* `2`: Location is `Horde territory`, only Horde players can unlock it.

Example:

```sql
UPDATE acore_world.custom_travelers_recall_locations
SET faction = 0
WHERE area_id = 159;
```

> This configures Brill as a `Neutral territory`, allowing both Alliance and Horde players to unlock it.

---

### 2.4. Adding Custom Locations

Custom teleport locations can be added directly to:

```text
acore_world.custom_travelers_recall_locations
```

Example:

```sql
INSERT IGNORE INTO acore_world.custom_travelers_recall_locations
(
    area_id,
    name,
    map_id,
    position_x,
    position_y,
    position_z,
    orientation,
    faction,
    icon,
    required_level,
    cooldown
)
VALUES
(
    159,
    'Brill',
    0,
    2259.25,
    290.43,
    34.1137,
    2.503233,
    2,
    'Interface\\Icons\\Spell_Arcane_TeleportUnderCity',
    0,
    0
);
```

> This adds `Brill` as an unlockable location at `area_id` `159` for Horde players (`Faction: 2`), with no level requirement and no individual cooldown.

---

### 2.5. Column Explanation

| Column           | Description                                        |
| ---------------- | -------------------------------------------------- |
| `area_id`        | Area ID used to detect location discovery          |
| `name`           | Name displayed in the addon                        |
| `map_id`         | Destination map ID (Azeroth, Outland, etc)         |
| `position_x/y/z` | Destination coordinates                            |
| `orientation`    | Player facing direction after teleport             |
| `faction`        | Faction restriction                                |
| `icon`           | World of Warcraft icon path displayed in the addon |
| `required_level` | Minimum level required to unlock the location      |
| `cooldown`       | Individual cooldown in seconds                     |

---

### 2.6. Retrieving Area Information

To retrieve the information required for a custom location, stand at the desired location in-game and execute:

```text
.gps
```
> This requires GM privileges.

This provides information such as:

* Area ID
* Map ID
* Coordinates
* Orientation

> Use these values when creating a custom teleport location.

---

### 2.7. Initial Location Unlocks

Set `TravelersRecall.StartWithAllLocationsUnlocked` in the module configuration to control whether new characters start with all locations unlocked:

* `0` (default): Characters do not receive all locations automatically.
* `1`: On their first login, characters unlock every neutral location and every location assigned to their faction.

> It does not unlock locations for characters that already exist. For configured faction- and race-based starting locations, see section 2.8.
> Setting it to `0` does not remove locations already unlocked by this method.

---

### 2.8. Faction- and Race-Based Initial Locations

Set `TravelersRecall.InitialTeleportLocationsEnabled` to `1` to unlock configured locations when a character is created:

```ini
TravelersRecall.InitialTeleportLocationsEnabled = 1
TravelersRecall.InitialAllianceTeleports = 1519
TravelersRecall.InitialHordeTeleports = 1637

TravelersRecall.InitialHumanTeleports = 87
TravelersRecall.InitialDwarfTeleports = 1537
TravelersRecall.InitialNightElfTeleports = 1657
TravelersRecall.InitialGnomeTeleports = 1537
TravelersRecall.InitialDraeneiTeleports = 3557

TravelersRecall.InitialOrcTeleports = 362
TravelersRecall.InitialUndeadTeleports = 1497
TravelersRecall.InitialTaurenTeleports = 1638
TravelersRecall.InitialTrollTeleports = 380
TravelersRecall.InitialBloodElfTeleports = 3487
```

The faction setting applies to every character of that faction, and the race setting adds locations for that character's race. Values are area IDs from `custom_travelers_recall_locations`; configure multiple area_id as a comma-separated list, for example `1519,1537,1657`. The values above are the defaults in the distributed configuration.

> This feature is separate from `TravelersRecall.StartWithAllLocationsUnlocked`: it uses the configured faction and race lists at character creation, while the other option unlocks every neutral and faction location.

---

### 2.9. Cast Time Before Teleporting

Set `TravelersRecall.CastTime.Enable` to `1` to make players cast before they teleport:

```ini
TravelersRecall.CastTime.Enable = 1
```

* `0` (default): Players teleport instantly.
* `1`: Players cast "Translocate" for 3 seconds, with the cast bar and visual of the Blood Elf translocation orbs, then teleport.

Moving, jumping or pressing Escape cancels the cast. A cancelled cast does not teleport and does not start the cooldown. Mounted players are dismounted when the cast starts. Party bots teleport with the player when the cast finishes.

> The cast uses the "Translocate" spell (26566) from the Duskwither Spire orbs in Eversong Woods. The SQL update binds the module's spell script to it; the orbs themselves keep working as before.

---

## 3. Teleport to a Friend

The `Teleport to a Friend` option teleports the player to the currently selected character.

* If `TravelersRecall.TeleportToFriendEnabled` is set to `1` in the configuration file, players can teleport to a selected group member. A teleport option will appear in the addon list.

The teleport is allowed only when all of the following conditions are met for both players:

* The selected player is online.
* The selected player is in the same group.
* Neither player is on a transport.
* The selected player is not in flight, dead or in a battleground.
* Flight, stealth, combat, death, battleground, dungeon and raid restrictions follow the same `TravelersRecall.CanTeleport.*` settings as the other locations. The dungeon and raid setting applies to both players, so teleporting into a group member's dungeon or raid requires `TravelersRecall.CanTeleport.InDungeonOrRaid = 1`.
* The player meets the normal entry requirements of the selected player's map, such as level, attunement, lockouts and raid group.

The cooldown is **24 hours (86400 seconds)** by default and is configured in the database. To change it without editing the database, set `TravelersRecall.FriendCooldown` in the configuration file:

* `-1` (default): Uses the cooldown from the database.
* `0`: No cooldown.
* Greater than `0`: Cooldown in seconds, for example `3600` for one hour.

> This setting only affects Teleport to a Friend. Other locations keep their own cooldowns.

## 4. GM Commands

The module provides GM commands for testing and manually unlocking locations.

### 4.1. Unlock a Specific Location

```text
.tr learn <area_id>
```

Example:

```text
.tr learn 159
```

This unlocks `Brill` for the selected player or yourself.

### 4.2. Unlock All Locations

```text
.tr learn all
```

Unlocks all available Traveler's Recall locations for the selected character.

### 4.3. Remove a Specific Location

```text
.tr remove <area_id>
```

Example:

```text
.tr remove 159
```

This removes `Brill` from the selected player or yourself.

### 4.4. Remove All Locations

```text
.tr remove all
```

Removes all Traveler's Recall locations from the selected player or yourself.

---

## 5. Notes

* Icons use standard World of Warcraft client icon paths.

---

## 6. FAQ

Q: Can I contact you?

A: Yes!

Q: I have an idea for the module!

A: That's not a question, but feel free to open a feature request!

---

## 7. Author

**ThiagoSBarreto**
LesmaSamuray

---

## 8. Credits

* Fork maintained by [EvilTactician](https://github.com/EvilTactician), based on [ThiagoSBarreto/mod-travelers-recall](https://github.com/ThiagoSBarreto/mod-travelers-recall)
* Quest hubs and location fixes from [Tntdruid's fork](https://github.com/Tntdruid/mod-travelers-recall)

---

## 9. Update

To update the module, follow these instructions:

* Go to the module folder:
   ```text
   cd /path/to/azerothcore/modules
   ```
* Update the repository:
   ```text
   git pull
   ```
* Rebuild your AzerothCore installation.
* Copy the new configuration.

---

## 10. Change Log

### 19/11/2026 - Version 1.4.0

* Added 49 quest hubs from [Tntdruid's fork](https://github.com/Tntdruid/mod-travelers-recall/commit/b04b4ac93d836646088bf13941a63d3ac0eb0a58): Hammerfall, Stonard, Grom'gol Base Camp, Rebel Camp, Kargath, Flame Crest, Aerie Peak, Revantusk Village, Chillwind Camp, Light's Hope Chapel, Falconwing Square, Fairbreeze Village, Tranquillien, Shattered Sun Staging Area, Sen'jin Village, Theramore Isle, Mudsprocket, Thalanaar, Forest Song, Talrendis Point, Talonbranch Glade, Mag'har Post, Sporeggar, Garadar, Aeris Landing, Vengeance Landing, New Agamand, Camp Winterhoof, Taunka'le Village, Bor'gorok Outpost, Agmar's Hammer, Venomspite, Conquest Hold, Camp Oneqwah, Grom'arsh Crash-Site, Camp Tunka'lo, Fizzcrank Airstrip, Stars' Rest, Wintergarde Keep, Fordragon Hold, Frosthold, Unu'pe, Wyrmrest Temple, Nesingwary Base Camp, River's Heart, K3, Brunnhildar Village, Dun Niffelem and The Shadow Vault
* Added 14 flight points that had no location: The Bulwark, Thondroril River, Shatter Point, Apothecary Camp, Moa'ki Harbor, Amber Ledge, Transitus Shield, Kor'kron Vanguard, Windrunner's Overlook, Sunreaver's Command, The Argent Vanguard, Crusaders' Pinnacle, Death's Rise and Bouldercrag's Refuge
* Fixed Warsong Hold: it used Wildhammer Stronghold's area ID and Shadowmoon Valley coordinates, so it never unlocked in Borean Tundra and teleported to an invalid spot
* Fixed The Exodar: it teleported to Ironforge's coordinates on the Outland map
* Fixed Wildhammer Stronghold unlocking anywhere in Shadowmoon Valley instead of at the stronghold
* Fixed the Tharllmar typo (now Thrallmar) and Mirage Raceway's icon, made Mirage Raceway neutral, and corrected the Brackenwall Village and Telaar icons

### 14/11/2026 - Version 1.3.1

* Added Shadowprey Village and Nijel's Point

### 13/11/2026 - Version 1.3.0

* Teleport to a Friend follows the `TravelersRecall.CanTeleport.*` settings like the other locations, instead of always blocking flight, death, battlegrounds, dungeons and raids (see section 3)
* Teleport to a Friend checks the normal entry requirements of the target's map, and no longer starts the cooldown when the teleport fails
* Added Tarren Mill, Sun Rock Retreat and Stonetalon Peak

### 09/11/2026 - Version 1.2.0

* **New feature:** Cast time before teleporting
    * If enabled in the configuration file (`TravelersRecall.CastTime.Enable`), players cast "Translocate" for 3 seconds before teleporting (see section 2.9)
    * Moving, jumping or pressing Escape cancels the cast without starting the cooldown
* **New feature:** Separate cooldown for Teleport to a Friend (`TravelersRecall.FriendCooldown`, see section 3)
* Fixed compile errors on the Playerbots branch of AzerothCore ([#34](https://github.com/ThiagoSBarreto/mod-travelers-recall/issues/34))

### 27/10/2026 - Version 1.1.1

* Fixed bots not teleporting with the party leader
* Fixed sub-zones discovery, thanks to [EvilTactician](https://github.com/EvilTactician)

### 18/10/2026 - Version 1.1.0

* Fixed SQL hard coded database names, thanks to [alexanderschwarzkopf8](https://github.com/alexanderschwarzkopf8)
* Now the modules uses cache, worldserver restart is required after database changes
* Fixed some Northrend locations using wrong map_id
* Bots no longer unlock teleport locations
    * Headless clients still do

### 10/10/2026 - Version 1.0.0 is live

* Additional code refactoring
* Added Outland and Northrend teleport locations
* Added command:
    * `.tr cd` (shows remaining cooldowns for all teleports)
* Added GM commands:
    * `.tr cd reset <area_id>`
    * `.tr cd reset all`

* **New feature:** Initial teleports
    * Players can start with predefined locations unlocked
    * Faction- and race-based initial unlocks are configurable
    * Players can start with all neutral and faction locations unlocked on first login

* **New feature:** Teleport to a Friend
    * If enabled in the configuration file and all conditions are met (see section 3), this teleports the player to a selected group member.

---

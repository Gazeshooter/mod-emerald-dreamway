-- Verdant Fields graveyard and Spirit Healer.
-- The PlayerScript overrides releases on map 169 to graveyard ID 990101.

DELETE FROM `graveyard_zone`
WHERE `ID` = 990101;

DELETE FROM `game_graveyard`
WHERE `ID` = 990101;

INSERT INTO `game_graveyard`
    (`ID`, `Map`, `x`, `y`, `z`, `Comment`)
VALUES
    (
        990101,
        169,
        -2066.5056,
        -924.2007,
        131.61397,
        'Emerald Dreamway - Verdant Fields'
    );

INSERT INTO `graveyard_zone`
    (`ID`, `GhostZone`, `Faction`)
VALUES
    (990101, 956, 0);

-- Remove previous Spirit Healer spawns and attached addon/pathing data
-- from map 169 before inserting one stationary healer.
DELETE `ca`
FROM `creature_addon` AS `ca`
INNER JOIN `creature` AS `c`
    ON `c`.`guid` = `ca`.`guid`
LEFT JOIN `creature_template` AS `ct`
    ON `ct`.`entry` = `c`.`id`
WHERE `c`.`map` = 169
  AND (
      `c`.`id` = 6491
      OR (`c`.`npcflag` & 16384) <> 0
      OR (`ct`.`npcflag` & 16384) <> 0
  );

DELETE `c`
FROM `creature` AS `c`
LEFT JOIN `creature_template` AS `ct`
    ON `ct`.`entry` = `c`.`id`
WHERE `c`.`map` = 169
  AND (
      `c`.`id` = 6491
      OR (`c`.`npcflag` & 16384) <> 0
      OR (`ct`.`npcflag` & 16384) <> 0
  );

SET @EDW_SPIRIT_HEALER_GUID :=
    (SELECT COALESCE(MAX(`guid`), 0) + 1 FROM `creature`);

INSERT INTO `creature`
(
    `guid`,
    `id`,
    `map`,
    `zoneId`,
    `areaId`,
    `spawnMask`,
    `phaseMask`,
    `equipment_id`,
    `position_x`,
    `position_y`,
    `position_z`,
    `orientation`,
    `spawntimesecs`,
    `wander_distance`,
    `currentwaypoint`,
    `curhealth`,
    `curmana`,
    `MovementType`,
    `npcflag`,
    `unit_flags`,
    `dynamicflags`,
    `ScriptName`,
    `VerifiedBuild`,
    `CreateObject`,
    `Comment`
)
VALUES
(
    @EDW_SPIRIT_HEALER_GUID,
    6491,
    169,
    956,
    956,
    1,
    1,
    0,
    -2073.7546,
    -925.57733,
    132.13492,
    1.69538,
    300,
    0,
    0,
    1,
    0,
    0,
    16384,
    0,
    0,
    '',
    NULL,
    0,
    'Emerald Dreamway - Verdant Fields Spirit Healer'
);

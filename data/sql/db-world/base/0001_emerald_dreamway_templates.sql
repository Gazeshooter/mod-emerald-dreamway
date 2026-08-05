-- Emerald Dreamway GameObject templates.

DELETE FROM `gameobject_template`
WHERE `entry` IN (990001, 990002, 990003, 990004, 990005, 990006, 990007, 990008, 990009, 990010, 990011, 990012);

INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `size`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`, `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`, `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`, `AIName`, `ScriptName`, `VerifiedBuild`) VALUES
(990001,10,249,'Dreamway Pedestal - Twilight Grove','Interact','','',1,0,0,0,0,0,0,0,0,0,0,60957,0,0,0,0,0,0,1,0,0,0,0,0,0,'','go_emerald_dreamway_pedestal',NULL),
(990002,10,249,'Dreamway Pedestal - Verdant Fields to Twilight Grove','Interact','','',1,0,0,0,0,0,0,0,0,0,0,60957,0,0,0,0,0,0,1,0,0,0,0,0,0,'','go_emerald_dreamway_pedestal',NULL),
(990003,10,249,'Dreamway Pedestal - Bough Shadow','Interact','','',1,0,0,0,0,0,0,0,0,0,0,60957,0,0,0,0,0,0,1,0,0,0,0,0,0,'','go_emerald_dreamway_pedestal',NULL),
(990004,10,249,'Dreamway Pedestal - Verdant Fields to Bough Shadow','Interact','','',1,0,0,0,0,0,0,0,0,0,0,60957,0,0,0,0,0,0,1,0,0,0,0,0,0,'','go_emerald_dreamway_pedestal',NULL),
(990005,10,249,'Dreamway Pedestal - Dream Bough','Interact','','',1,0,0,0,0,0,0,0,0,0,0,60957,0,0,0,0,0,0,1,0,0,0,0,0,0,'','go_emerald_dreamway_pedestal',NULL),
(990006,10,249,'Dreamway Pedestal - Verdant Fields to Dream Bough','Interact','','',1,0,0,0,0,0,0,0,0,0,0,60957,0,0,0,0,0,0,1,0,0,0,0,0,0,'','go_emerald_dreamway_pedestal',NULL),
(990007,10,249,'Dreamway Pedestal - Seradane','Interact','','',1,0,0,0,0,0,0,0,0,0,0,60957,0,0,0,0,0,0,1,0,0,0,0,0,0,'','go_emerald_dreamway_pedestal',NULL),
(990008,10,249,'Dreamway Pedestal - Verdant Fields to Seradane','Interact','','',1,0,0,0,0,0,0,0,0,0,0,60957,0,0,0,0,0,0,1,0,0,0,0,0,0,'','go_emerald_dreamway_pedestal',NULL);

-- Verdant Fields destination markers.
-- Each marker copies only the display model from an existing stock AzerothCore template
-- and uses an inert generic GameObject template.

INSERT INTO `gameobject_template`
(
    `entry`, `type`, `displayId`, `name`,
    `IconName`, `castBarCaption`, `unk1`, `size`,
    `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`,
    `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`,
    `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`,
    `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`,
    `AIName`, `ScriptName`, `VerifiedBuild`
)
SELECT
    990009, 5, `displayId`, 'Dreamway Marker - Twilight Grove',
    '', '', '', 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '', '', NULL
FROM `gameobject_template`
WHERE `entry` = 188343;

INSERT INTO `gameobject_template`
(
    `entry`, `type`, `displayId`, `name`,
    `IconName`, `castBarCaption`, `unk1`, `size`,
    `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`,
    `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`,
    `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`,
    `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`,
    `AIName`, `ScriptName`, `VerifiedBuild`
)
SELECT
    990010, 5, `displayId`, 'Dreamway Marker - Bough Shadow',
    '', '', '', 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '', '', NULL
FROM `gameobject_template`
WHERE `entry` = 181913;

INSERT INTO `gameobject_template`
(
    `entry`, `type`, `displayId`, `name`,
    `IconName`, `castBarCaption`, `unk1`, `size`,
    `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`,
    `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`,
    `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`,
    `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`,
    `AIName`, `ScriptName`, `VerifiedBuild`
)
SELECT
    990011, 5, `displayId`, 'Dreamway Marker - Dream Bough',
    '', '', '', 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '', '', NULL
FROM `gameobject_template`
WHERE `entry` = 184092;

INSERT INTO `gameobject_template`
(
    `entry`, `type`, `displayId`, `name`,
    `IconName`, `castBarCaption`, `unk1`, `size`,
    `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`,
    `Data6`, `Data7`, `Data8`, `Data9`, `Data10`, `Data11`,
    `Data12`, `Data13`, `Data14`, `Data15`, `Data16`, `Data17`,
    `Data18`, `Data19`, `Data20`, `Data21`, `Data22`, `Data23`,
    `AIName`, `ScriptName`, `VerifiedBuild`
)
SELECT
    990012, 5, `displayId`, 'Dreamway Marker - Seradane',
    '', '', '', 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '', '', NULL
FROM `gameobject_template`
WHERE `entry` = 176591;

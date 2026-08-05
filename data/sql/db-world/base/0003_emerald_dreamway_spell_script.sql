-- Bind the pedestal activation spell to the module SpellScript.

DELETE FROM `spell_script_names`
WHERE `spell_id` = 60957
  AND `ScriptName` = 'spell_emerald_dreamway_activation';

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`)
VALUES (60957, 'spell_emerald_dreamway_activation');

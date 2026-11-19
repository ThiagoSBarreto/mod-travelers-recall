-- Cast bar before teleporting (TravelersRecall.CastTime.Enable)
-- Binds the module's spell script to "Translocate" (26566, Duskwither Spire orb in
-- Eversong Woods). The script only changes casts started by Traveler's Recall;
-- using the orb in game works as before.
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_travelers_recall_cast';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(26566, 'spell_travelers_recall_cast');

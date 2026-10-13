-- Icons change
UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Ability_Druid_TravelForm'
WHERE area_id = 2361 AND map_id = 1;

-- New locations
INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3576, 'Azure Watch', 530, -4190.85, -12516.5, 44.5256, 1.34225, 1, 'Interface\\Icons\\Spell_Arcane_TeleportExodar', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3584, 'Blood Watch', 530, -1944.5, -11873.7, 49.3983, 6.04835, 1, 'Interface\\Icons\\Spell_Arcane_TeleportExodar', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(186, 'Dolanaar', 1, 9848.37, 966.953, 1306.38, 3.77457, 1, 'Interface\\Icons\\Spell_Arcane_TeleportDarnassus', 0, 0);
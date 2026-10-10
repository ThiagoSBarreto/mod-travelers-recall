-- Teleport to Friend
INSERT IGNORE INTO acore_world.custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(-1, 'Teleport to Friend', -1, -1, -1, -1, -1, 0, 'Interface\\Icons\\Achievement_BG_tophealer_AB', 0, 86400);

-- Adds Teleport to Friend for existing characters
INSERT IGNORE INTO acore_characters.custom_travelers_recall_unlocks
	(guid, location_id, unlocked_at, cooldown_end)
SELECT characters.guid, locations.id, NOW(), 0
FROM acore_characters.characters
CROSS JOIN acore_world.custom_travelers_recall_locations AS locations
LEFT JOIN acore_characters.custom_travelers_recall_unlocks AS unlocks
	ON unlocks.guid = characters.guid
	AND unlocks.location_id = locations.id
WHERE locations.area_id = -1
	AND locations.map_id = -1
	AND locations.name = 'Teleport to Friend'
	AND unlocks.guid IS NULL;
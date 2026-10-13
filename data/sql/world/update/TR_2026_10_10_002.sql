-- Teleport to Friend
INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(-1, 'Teleport to Friend', -1, -1, -1, -1, -1, 0, 'Interface\\Icons\\Achievement_BG_tophealer_AB', 0, 86400);

-- Characters that existed before this location was added get it on their next
-- login (TravelersRecallPlayerScript::OnPlayerLogin). This file runs against the
-- world database, which cannot name the characters database portably.
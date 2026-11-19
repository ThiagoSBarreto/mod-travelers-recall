-- The Bulwark's sign-post position is in area 152 (The Bulwark, Tirisfal Glades),
-- not 813 on the Western Plaguelands side, so it never unlocked.
UPDATE IGNORE custom_travelers_recall_locations
SET area_id = 152, icon = 'Interface\\Icons\\Achievement_Zone_TirisfalGlades_01'
WHERE name = 'The Bulwark' AND area_id = 813;

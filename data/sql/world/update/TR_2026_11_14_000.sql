-- ---------------------------------------------------------------------------
-- Data fixes
-- ---------------------------------------------------------------------------

-- Warsong Hold was stored with area_id 3745 (Wildhammer Stronghold, Outland) and
-- Shadowmoon Valley coordinates on map 571, so it could only be unlocked by walking
-- into an Alliance base in Outland and teleported players to an invalid spot.
-- The earlier fix in TR_2026_10_10_001 targeted area_id 3537, which never matched.
UPDATE custom_travelers_recall_locations
SET area_id = 4129, map_id = 571, position_x = 2741.29, position_y = 6097.16, position_z = 76.9055, orientation = 0.731543
WHERE name = 'Warsong Hold' AND area_id = 3745;

-- Wildhammer Stronghold used 3520 (the whole Shadowmoon Valley zone). Point it at
-- its own sub-zone now that 3745 is free.
UPDATE IGNORE custom_travelers_recall_locations
SET area_id = 3745
WHERE name = 'Wildhammer Stronghold' AND area_id = 3520;

-- Typos
UPDATE custom_travelers_recall_locations SET name = 'Thrallmar' WHERE area_id = 3536 AND name = 'Tharllmar';
UPDATE custom_travelers_recall_locations SET icon = 'Interface\\Icons\\Achievement_Zone_Tanaris_01' WHERE area_id = 2240;

-- Wrong zone icons
UPDATE custom_travelers_recall_locations SET icon = 'Interface\\Icons\\Achievement_Zone_DustwallowMarsh' WHERE area_id = 496;
UPDATE custom_travelers_recall_locations SET icon = 'Interface\\Icons\\Achievement_Zone_Nagrand_01' WHERE area_id = 3626;

-- Mirage Raceway is a neutral goblin/gnome outpost
UPDATE custom_travelers_recall_locations SET faction = 0 WHERE area_id = 2240;

-- ---------------------------------------------------------------------------
-- Quest hubs
--   Coordinates come from game_tele, area IDs from AreaTable.dbc (3.3.5a).
--   faction: 0 = neutral, 1 = Alliance, 2 = Horde
-- ---------------------------------------------------------------------------
INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES
(272, 'Tarren Mill', 0, -34.1467, -923.366, 54.5576, 0.15019, 2, 'Interface\\Icons\\Achievement_Zone_HillsbradFoothills', 0, 0),
(321, 'Hammerfall', 0, -941.007, -3526.66, 70.935, 3.48668, 2, 'Interface\\Icons\\Achievement_Zone_ArathiHighlands_01', 0, 0),
(75, 'Stonard', 0, -10446.9, -3261.91, 20.1795, 5.02142, 2, 'Interface\\Icons\\Achievement_Zone_SwampSorrows_01', 0, 0),
(117, 'Grom''gol Base Camp', 0, -12388.9, 172.578, 2.83358, 1.91753, 2, 'Interface\\Icons\\Achievement_Zone_Stranglethorn_01', 0, 0),
(99, 'Rebel Camp', 0, -11322.4, -202.492, 75.6362, 0.432339, 1, 'Interface\\Icons\\Achievement_Zone_Stranglethorn_01', 0, 0),
(340, 'Kargath', 0, -6692.48, -2175.31, 244.145, 0.427571, 2, 'Interface\\Icons\\Achievement_Zone_Badlands_01', 0, 0),
(251, 'Flame Crest', 0, -7501.51, -2183.08, 165.926, 6.07144, 2, 'Interface\\Icons\\Achievement_Zone_BurningSteppes', 0, 0),
(348, 'Aerie Peak', 0, 260.366, -2125.21, 119.565, 3.18494, 1, 'Interface\\Icons\\Achievement_Zone_Hinterlands_01', 0, 0),
(3317, 'Revantusk Village', 0, -557.226, -4581.27, 9.5884, 1.01724, 2, 'Interface\\Icons\\Achievement_Zone_Hinterlands_01', 0, 0),
(3197, 'Chillwind Camp', 0, 967.964, -1443.99, 65.0399, 2.05398, 1, 'Interface\\Icons\\Achievement_Zone_WesternPlaguelands_01', 0, 0),
(2268, 'Light''s Hope Chapel', 0, 2279.65, -5310.01, 87.0759, 5.07618, 0, 'Interface\\Icons\\Achievement_Zone_EasternPlaguelands', 0, 0),
(3665, 'Falconwing Square', 530, 9514.33, -6822.1, 16.4907, 1.60979, 2, 'Interface\\Icons\\Achievement_Zone_EversongWoods', 0, 0),
(3462, 'Fairbreeze Village', 530, 8714.14, -6650.33, 72.7517, 2.39205, 2, 'Interface\\Icons\\Achievement_Zone_EversongWoods', 0, 0),
(3488, 'Tranquillien', 530, 7564.25, -6872.23, 96.0413, 4.3579, 2, 'Interface\\Icons\\Achievement_Zone_Ghostlands', 0, 0),
(4085, 'Shattered Sun Staging Area', 530, 12947.4, -6893.31, 5.68398, 3.09154, 0, 'Interface\\Icons\\Achievement_Zone_IsleOfQuelDanas', 0, 0),
(367, 'Sen''jin Village', 1, -813.097, -4880.08, 18.995, 4.42647, 2, 'Interface\\Icons\\Achievement_Zone_Durotar', 0, 0),
(513, 'Theramore Isle', 1, -3641.3, -4358.93, 8.35467, 3.81559, 1, 'Interface\\Icons\\Achievement_Zone_DustwallowMarsh', 0, 0),
(4010, 'Mudsprocket', 1, -4573.79, -3173.15, 34.0877, 3.1231, 0, 'Interface\\Icons\\Achievement_Zone_DustwallowMarsh', 0, 0),
(467, 'Stonetalon Peak', 1, 2678.38, 1497.46, 233.869, 6.26038, 1, 'Interface\\Icons\\Achievement_Zone_Stonetalon_01', 0, 0),
(460, 'Sun Rock Retreat', 1, 966.147, 926.499, 104.649, 1.27231, 2, 'Interface\\Icons\\Achievement_Zone_Stonetalon_01', 0, 0),
(608, 'Nijel''s Point', 1, 176.426, 1309.76, 190.18, 0.556817, 1, 'Interface\\Icons\\Achievement_Zone_Desolace', 0, 0),
(2408, 'Shadowprey Village', 1, -1664.79, 3091.67, 30.5552, 6.07818, 2, 'Interface\\Icons\\Achievement_Zone_Desolace', 0, 0),
(489, 'Thalanaar', 1, -4525.63, -791.364, -42.3639, 1.09938, 1, 'Interface\\Icons\\Achievement_Zone_Feralas', 0, 0),
(2358, 'Forest Song', 1, 3011.16, -3359.08, 147.96, 2.42202, 1, 'Interface\\Icons\\Achievement_Zone_Ashenvale_01', 0, 0),
(3137, 'Talrendis Point', 1, 2735.06, -3867.44, 98.6548, 3.56139, 1, 'Interface\\Icons\\Achievement_Zone_Azshara_01', 0, 0),
(1998, 'Talonbranch Glade', 1, 6209.51, -1927.01, 569.393, 3.82137, 1, 'Interface\\Icons\\Achievement_Zone_Felwood', 0, 0),
(3555, 'Mag''har Post', 530, 512.08, 3879.32, 191.494, 0.027839, 2, 'Interface\\Icons\\Achievement_Zone_HellfirePeninsula_01', 0, 0),
(3649, 'Sporeggar', 530, 209.095, 8547.4, 23.1083, 5.33054, 0, 'Interface\\Icons\\Achievement_Zone_Zangarmarsh', 0, 0),
(3613, 'Garadar', 530, -1321.34, 7239.12, 32.7371, 4.04169, 2, 'Interface\\Icons\\Achievement_Zone_Nagrand_01', 0, 0),
(3623, 'Aeris Landing', 530, -2059.85, 8550.89, 24.0587, 1.944, 0, 'Interface\\Icons\\Achievement_Zone_Nagrand_01', 0, 0),
(4000, 'Vengeance Landing', 571, 1942.86, -6167.11, 23.724, 2.64258, 2, 'Interface\\Icons\\Achievement_Zone_HowlingFjord_01', 0, 0),
(3991, 'New Agamand', 571, 424.405, -4548.76, 245.652, 5.37325, 2, 'Interface\\Icons\\Achievement_Zone_HowlingFjord_01', 0, 0),
(4018, 'Camp Winterhoof', 571, 2649.82, -4362.69, 276.885, 5.15979, 2, 'Interface\\Icons\\Achievement_Zone_HowlingFjord_01', 0, 0),
(4037, 'Taunka''le Village', 571, 3464.17, 4087.16, 17.0561, 2.02607, 2, 'Interface\\Icons\\Achievement_Zone_BoreanTundra_01', 0, 0),
(4122, 'Bor''gorok Outpost', 571, 4488.76, 5736.18, 80.0769, 5.45078, 2, 'Interface\\Icons\\Achievement_Zone_BoreanTundra_01', 0, 0),
(4165, 'Agmar''s Hammer', 571, 3841.51, 1534.04, 89.7246, 4.78105, 2, 'Interface\\Icons\\Achievement_Zone_Dragonblight_01', 0, 0),
(4186, 'Venomspite', 571, 3241.29, -699.767, 166.985, 5.3583, 2, 'Interface\\Icons\\Achievement_Zone_Dragonblight_01', 0, 0),
(4206, 'Conquest Hold', 571, 3251.86, -2244.98, 114.555, 1.20122, 2, 'Interface\\Icons\\Achievement_Zone_GrizzlyHills_01', 0, 0),
(4211, 'Camp Oneqwah', 571, 3848.7, -4543.46, 209.278, 1.46236, 2, 'Interface\\Icons\\Achievement_Zone_GrizzlyHills_01', 0, 0),
(4429, 'Grom''arsh Crash-Site', 571, 7843.9, -796.826, 1183.4, 4.79714, 2, 'Interface\\Icons\\Achievement_Zone_StormPeaks_01', 0, 0),
(4441, 'Camp Tunka''lo', 571, 7808.82, -2949.29, 1257.53, 4.11778, 2, 'Interface\\Icons\\Achievement_Zone_StormPeaks_01', 0, 0),
(4108, 'Fizzcrank Airstrip', 571, 4147.98, 5278.79, 24.7334, 0.150308, 1, 'Interface\\Icons\\Achievement_Zone_BoreanTundra_02', 0, 0),
(4158, 'Stars'' Rest', 571, 3480.7, 2000.06, 64.975, 5.92773, 1, 'Interface\\Icons\\Achievement_Zone_Dragonblight_01', 0, 0),
(4177, 'Wintergarde Keep', 571, 3682.71, -722.635, 212.729, 5.7991, 1, 'Interface\\Icons\\Achievement_Zone_Dragonblight_01', 0, 0),
(4169, 'Fordragon Hold', 571, 4595.02, 1435.48, 189.187, 0.064754, 1, 'Interface\\Icons\\Achievement_Zone_Dragonblight_01', 0, 0),
(4428, 'Frosthold', 571, 6666.43, -211.341, 947.816, 1.56526, 1, 'Interface\\Icons\\Achievement_Zone_StormPeaks_01', 0, 0),
(4113, 'Unu''pe', 571, 2925.02, 4065.63, 1.46737, 3.56739, 0, 'Interface\\Icons\\Achievement_Zone_BoreanTundra_03', 0, 0),
(4161, 'Wyrmrest Temple', 571, 3556.22, 264.514, 342.722, 2.53185, 0, 'Interface\\Icons\\Achievement_Zone_Dragonblight_01', 0, 0),
(4284, 'Nesingwary Base Camp', 571, 5561.69, 5748.65, -76.2847, 1.51591, 0, 'Interface\\Icons\\Achievement_Zone_Sholazar_01', 0, 0),
(4290, 'River''s Heart', 571, 5368.05, 4843.58, -197.195, 5.93375, 0, 'Interface\\Icons\\Achievement_Zone_Sholazar_01', 0, 0),
(4418, 'K3', 571, 6123.7, -1059.19, 402.624, 4.65975, 0, 'Interface\\Icons\\Achievement_Zone_StormPeaks_01', 0, 0),
(4422, 'Brunnhildar Village', 571, 7056.37, -1698, 821.595, 3.61516, 0, 'Interface\\Icons\\Achievement_Zone_StormPeaks_01', 0, 0),
(4438, 'Dun Niffelem', 571, 7165.42, -2729.01, 776.77, 0.347884, 0, 'Interface\\Icons\\Achievement_Zone_StormPeaks_01', 0, 0),
(4477, 'The Shadow Vault', 571, 8427.88, 2706.33, 655.095, 5.743, 0, 'Interface\\Icons\\Achievement_Zone_IceCrown_01', 0, 0);

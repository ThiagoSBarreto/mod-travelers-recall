-- Correct IDs and Coordinations to Warsong Hold
UPDATE custom_travelers_recall_locations
SET map_id=571, position_x=2741.29, position_y=6097.16, position_z=76.9055, orientation=0.731543
WHERE area_id=3537;

-- Correct MAP_ID for Razor Hill
UPDATE custom_travelers_recall_locations
SET map_id=1
WHERE area_id=362;

-- Correct ICON for Freewind Post
UPDATE custom_travelers_recall_locations
SET icon='Interface\\Icons\\Achievement_Zone_ThousandNeedles_01'
WHERE area_id=484;

-- Inserts OUTLAND - Neutral
INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3565, 'Cenarion Refuge', 530, -223.541, 5487.99, 23.2281, 0.886755, 0, 'Interface\\Icons\\Achievement_Zone_Zangarmarsh', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3539, 'The Stair of Destiny', 530, -248.149, 921.875, 84.3797, 1.58415, 0, 'Interface\\Icons\\Achievement_Zone_HellfirePeninsula_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3738, 'Stormspire', 530, 4150.19, 3015.92, 339.188, 4.0499, 0, 'Interface\\Icons\\Achievement_Zone_Netherstorm_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3951, 'Evergrove', 530, 2976.85, 5511.01, 144.637, 3.86042, 0, 'Interface\\Icons\\Achievement_Zone_BladesEdgeMtns_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3786, 'Ogri''la', 530, 2297.68, 7293.12, 365.617, 1.28888, 0, 'Interface\\Icons\\Achievement_Zone_BladesEdgeMtns_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3724, 'Cosmowrench', 530, 2988.21, 1806.9, 139.071, 3.8591, 0, 'Interface\\Icons\\Achievement_Zone_Netherstorm_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3938, 'Sanctum of the Stars', 530, -4115.51, 1120.54, 44.5242, 2.6318, 0, 'Interface\\Icons\\Achievement_Zone_Shadowmoon', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3754, 'Altar of Sha''tar', 530, -3053.96, 828.896, -10.4689, 5.53466, 0, 'Interface\\Icons\\Achievement_Zone_Shadowmoon', 0, 0);

-- Inserts OUTLAND - Alliance
INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3538, 'Honor Hold', 530, -748.211, 2681.52, 100.35, 5.7479, 1, 'Interface\\Icons\\Achievement_Zone_HellfirePeninsula_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3552, 'Temple of Telhamat', 530, 78.9769, 4333.58, 101.553, 0.011347, 1, 'Interface\\Icons\\Achievement_Zone_HellfirePeninsula_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3644, 'Telredor', 530, 278.582, 6001.27, 144.73, 1.53156, 1, 'Interface\\Icons\\Achievement_Zone_Zangarmarsh', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3766, 'Orebor Harborage', 530, 958.66, 7374.02, 27.9079, 6.12064, 1, 'Interface\\Icons\\Achievement_Zone_Zangarmarsh', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3772, 'Sylvanaar', 530, 2018.91, 6854.47, 171.409, 0.08726, 1, 'Interface\\Icons\\Achievement_Zone_BladesEdgeMtns_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3918, 'Toshley''s Station', 530, 1910.63, 5556.25, 263.017, 6.25981, 1, 'Interface\\Icons\\Achievement_Zone_BladesEdgeMtns_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3626, 'Telaar', 530, -2560.76, 7300.72, 13.9485, 2.18422, 1, 'Interface\\Icons\\Achievement_Zone_BladesEdgeMtns_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3684, 'Allerian Stronghold', 530, -2949.27, 3958.32, 0.285455, 1.37205, 1, 'Interface\\Icons\\Achievement_Zone_Terrokar', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3745, 'Wildhammer Stronghold', 530, -3989.47, 2168.39, 105.35, 3.08422, 1, 'Interface\\Icons\\Achievement_Zone_Shadowmoon', 0, 0);

-- Inserts OUTLAND - Horde
INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3683, 'Stonebreaker Hold', 530, -2640.12, 4404.32, 35.103, 4.14529, 2, 'Interface\\Icons\\Achievement_Zone_Terrokar', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3645, 'Zabra''jin', 530, 260.28, 7860.4, 23.3231, 3.77545, 2, 'Interface\\Icons\\Achievement_Zone_Zangarmarsh', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3718, 'Swamprat Post', 530, 104.534, 5199.31, 21.1033, 4.22156, 2, 'Interface\\Icons\\Achievement_Zone_Zangarmarsh', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3554, 'Falcon Watch', 530, -600.782, 4100.1, 90.5571, 0.207697, 2, 'Interface\\Icons\\Achievement_Zone_HellfirePeninsula_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3536, 'Tharllmar', 530, 156.251, 2673.45, 85.1587, 0.382074, 2, 'Interface\\Icons\\Achievement_Zone_HellfirePeninsula_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3812, 'Spinebreaker Post', 530, -1326.36, 2371.99, 88.9503, 6.19165, 2, 'Interface\\Icons\\Achievement_Zone_HellfirePeninsula_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3769, 'Thunderlord Stronghold', 530, 2314.75, 6041.96, 142.417, 6.24317, 2, 'Interface\\Icons\\Achievement_Zone_BladesEdgeMtns_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES(3844, 'Mok''Nathal Village', 530, 2210.93, 4763.72, 157.42, 4.60576, 2, 'Interface\\Icons\\Achievement_Zone_BladesEdgeMtns_01', 0, 0);

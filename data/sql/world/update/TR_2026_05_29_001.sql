-- Fixing The Exodar location
UPDATE custom_travelers_recall_locations
SET area_id = 3557, position_x = -3965.7, position_y = -11653.6, position_z = -138.844, orientation = 0.852154
WHERE area_id = 1357 and map_id = 530;

-- Removing duplicates
DELETE t1
FROM custom_travelers_recall_locations t1
INNER JOIN custom_travelers_recall_locations t2
ON t1.area_id = t2.area_id AND t1.id > t2.id;

CREATE TABLE IF NOT EXISTS `custom_travelers_recall_locations` (
  `id` int NOT NULL AUTO_INCREMENT,
  `area_id` int NOT NULL,
  `name` varchar(100) COLLATE utf8mb4_unicode_ci NOT NULL,
  `map_id` int NOT NULL,
  `position_x` float NOT NULL,
  `position_y` float NOT NULL,
  `position_z` float NOT NULL,
  `orientation` float NOT NULL DEFAULT '0',
  `faction` tinyint NOT NULL DEFAULT '2',
  `icon` varchar(100) COLLATE utf8mb4_unicode_ci DEFAULT NULL,
  `required_level` int DEFAULT '1',
  `cooldown` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `idx_area_id` (`area_id`)
);

-- New unique keys
SET @exists = (
    SELECT COUNT(*)
    FROM information_schema.statistics
    WHERE table_schema = DATABASE()
      AND table_name = 'custom_travelers_recall_locations'
      AND index_name = 'custom_travelers_recall_locations_unique'
);

SET @sql = IF(
    @exists = 0,
    'ALTER TABLE custom_travelers_recall_locations ADD UNIQUE KEY custom_travelers_recall_locations_unique (area_id)',
    'SELECT "Index already exists"'
);

PREPARE stmt FROM @sql;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;

-- New Locations
INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES
(108, 'Sentinel Hill', 0, -10624.5, 1096.66, 33.7622, 1.31041, 1, 'Interface\\Icons\\Achievement_Zone_Westfall_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES
(42, 'Darkshire', 0, -10573.0, -1182.51, 28.0136, 0.309022, 1, 'Interface\\Icons\\Achievement_Zone_Duskwood', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES
(69, 'Lakeshire', 0, -9266.59, -2188.77, 64.0892, 2.10205, 1, 'Interface\\Icons\\Achievement_Zone_RedridgeMountains', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES
(2418, 'Morgan''s Vigil', 0, -8372.77, -2754.46, 186.622, 3.43486, 1, 'Interface\\Icons\\Achievement_Zone_BurningSteppes', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES
(1446, 'Thorium Point', 0, -6506.47, -1149.95, 307.708, 4.18256, 1, 'Interface\\Icons\\Achievement_Zone_SearingGorge_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES
(1438, 'Nethergarde Keep', 0, -10999.8, -3380.08, 62.2525, 4.63501, 1, 'Interface\\Icons\\Achievement_Zone_BlastedLands_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES
(150, 'Menethil Harbor', 0, -3769.32, -744.26, 8.01027, 1.95752, 1, 'Interface\\Icons\\Achievement_Zone_Wetlands_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES
(320, 'Refuge Pointe', 0, -1246.61, -2529.32, 20.6098, 0.741709, 1, 'Interface\\Icons\\Achievement_Zone_ArathiHighlands_01', 0, 0);

INSERT IGNORE INTO custom_travelers_recall_locations
(area_id, name, map_id, position_x, position_y, position_z, orientation, faction, icon, required_level, cooldown)
VALUES
(271, 'Southshore', 0, -853.221, -533.529, 9.98556, 0.242866, 1, 'Interface\\Icons\\Achievement_Zone_HillsbradFoothills', 0, 0);
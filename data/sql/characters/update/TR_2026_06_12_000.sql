CREATE TABLE IF NOT EXISTS `custom_travelers_recall_unlocks` (
  `guid` int NOT NULL,
  `location_id` int NOT NULL,
  `unlocked_at` timestamp NULL DEFAULT CURRENT_TIMESTAMP,
  `cooldown_end` int unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`,`location_id`)
);

SET @exists = (
    SELECT COUNT(*)
    FROM information_schema.statistics
    WHERE table_schema = DATABASE()
      AND table_name = 'custom_travelers_recall_unlocks'
      AND index_name = 'uq_guid_location'
);

SET @sql = IF(
    @exists = 0,
    'ALTER TABLE custom_travelers_recall_unlocks ADD UNIQUE KEY uq_guid_location (guid, location_id)',
    'SELECT "Index already exists"'
);

PREPARE stmt FROM @sql;
EXECUTE stmt;
DEALLOCATE PREPARE stmt;
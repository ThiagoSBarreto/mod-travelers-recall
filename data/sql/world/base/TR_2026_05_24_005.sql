-- correcting z potion, now players won't fall to their deaths
UPDATE IGNORE custom_travelers_recall_locations
SET position_z = -213.75
WHERE area_id = 541 AND map_id = 1;

-- updating name to match
UPDATE IGNORE custom_travelers_recall_locations
SET name = 'Marshal''s Refuge'
WHERE area_id = 541 AND map_id = 1;

-- updating the icons for all locations
UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Stranglethorn_01'
WHERE area_id = 35 AND map_id = 0;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Tanaris_01'
WHERE area_id = 976 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Winterspring'
WHERE area_id = 2255 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Silithus_01'
WHERE area_id = 3425 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Netherstorm_01'
WHERE area_id = 3712 AND map_id = 530;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Shadowmoon'
WHERE area_id = 3744 AND map_id = 530;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Shadowmoon'
WHERE area_id = 3520 AND map_id = 530;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_BoreanTundra_01'
WHERE area_id = 3745 AND map_id = 571;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_BoreanTundra_02'
WHERE area_id = 4032 AND map_id = 571;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Reputation_ArgentCrusader'
WHERE area_id = 4658 AND map_id = 571;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Barrens_01'
WHERE area_id = 380 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_UnGoroCrater_01'
WHERE area_id = 541 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Barrens_01'
WHERE area_id = 392 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Feralas'
WHERE area_id = 1099 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Feralas'
WHERE area_id = 1116 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Ashenvale_01'
WHERE area_id = 431 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Ashenvale_01'
WHERE area_id = 415 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Ashenvale_01'
WHERE area_id = 2897 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Darkshore_01'
WHERE area_id = 448 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Darkshore_01'
WHERE area_id = 442 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Felwood'
WHERE area_id = 2479 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Felwood'
WHERE area_id = 1997 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Winterspring'
WHERE area_id = 2253 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Winterspring'
WHERE area_id = 2255 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Azshara_01'
WHERE area_id = 1237 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Moonglade'
WHERE area_id = 2361 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_TirisfalGlades_01'
WHERE area_id = 159 AND map_id = 0;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Durotar'
WHERE area_id = 362 AND map_id = 0;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Barrens_01'
WHERE area_id = 378 AND map_id = 1;

UPDATE IGNORE custom_travelers_recall_locations
SET icon = 'Interface\\Icons\\Achievement_Zone_Mulgore_01'
WHERE area_id = 222 AND map_id = 1;
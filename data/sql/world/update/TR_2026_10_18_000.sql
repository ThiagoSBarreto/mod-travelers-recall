-- Fix for some Northend areas with incorrect map_id
UPDATE IGNORE custom_travelers_recall_locations
SET map_id = 571
WHERE map_id = 495;
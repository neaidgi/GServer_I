 SET SQL_SAFE_UPDATES = 1;

UPDATE usercharacterinfo SET character_origin_code_first = NULL,
 character_code_first = NULL,character_jobname_first = NULL,
 character_nickname_first = NULL, character_level_first = NULL WHERE user_id = 123;
 
 UPDATE usercharacterinfo SET character_origin_code_second = NULL,
 character_code_second = NULL,character_jobname_second = NULL,
 character_nickname_second = NULL, character_level_second = NULL WHERE user_id = 123;
 
 UPDATE usercharacterinfo SET character_origin_code_third = NULL,
 character_code_third = NULL,character_jobname_third = NULL,
 character_nickname_third = NULL, character_level_third = NULL WHERE user_id = 123;
 
 SET SQL_SAFE_UPDATES = 1;
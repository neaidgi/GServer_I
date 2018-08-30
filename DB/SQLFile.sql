use userinfo;

create table CharacterInfo
(
	character_code INT NOT NULL primary key,
    character_name VARCHAR(20) NOT NULL,
    character_str INT NOT NULL,
    character_dex INT NOT NULL,
    character_int INT NOT NULL,
    character_health INT NOT NULL,
    character_attackpoint INT,
    character_defensepoint INT,
    character_speed FLOAT NOT NULL,
    character_growthhealth INT NOT NULL,
    character_growthstr INT NOT NULL,
    character_growthdex INT NOT NULL,
    character_growthint INT NOT NULL
);

use userinfo;

create table ItemInfo
(
	item_code INT NOT NULL,
	item_name VARCHAR(40) NOT NULL,
    item_type INT NOT NULL,
    item_equip_slot INT NOT NULL,
    item_damage INT,
    item_explanation VARCHAR(40),
    item_use_job INT
);

use userinfo;

create table UserCharacterInfo
(
	user_id VARCHAR(20) NOT NULL,
    
	charactercode_first INT,
    charactername_first VARCHAR(20),
    characternick_first VARCHAR(20),
    characterlevel_first INT,
    
    charactercode_seconde INT,
    charactername_seconde VARCHAR(20),
    characternick_seconde VARCHAR(20),
    characterlevel_seconde INT,
    
    charactercode_third INT,
    charactername_third VARCHAR(20),
    characternick_third VARCHAR(20),
    characterlevel_third INT,
    
    PRIMARY KEY(user_id),
	FOREIGN KEY(user_id) REFERENCES info(id),
    FOREIGN KEY(charactercode_first) REFERENCES CharacterInfo(character_code),
    FOREIGN KEY(charactercode_seconde) REFERENCES CharacterInfo(character_code),
    FOREIGN KEY(charactercode_third) REFERENCES CharacterInfo(character_code)
);


use userinfo;

INSERT INTO CharacterInfo VALUES (1,'Tanker',50,10,10,150,0,0,0.8,20,10,0,100),
								 (2,'Warrior',10,50,10,100,0,0,1.0,10,20,0,75),
								 (3,'Magician',10,10,50,50,0,0,1.2,5,5,20,50)
               
               
               
use userinfo;

INSERT INTO ItemInfo VALUES (1,'HealthPotion',1,1,1,'체력을 50 회복해준다',4),
                            (2,'Axe',2,1,2,'탱커의 기본무기',1),
                            (3,'Sword',2,1,2,'전사의 기본무기',2),
                            (4,'Staff',2,1,2,'마법사의 기본무기',3),
                            (5,'ClothAmor',3,1,3,'기본 천갑옷',4)
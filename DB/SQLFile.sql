create database userinfo;

use userinfo;

create table User(
	
	id varchar(20) not null primary key,

	pw varchar(20) not null
);

create table UserCharacterInfo(
	character_code varchar(20) not null primary key,

	character_origin_code INT not null,

	character_jobname VARCHAR(20) not null,

	character_nickname VARCHAR(20) not null,

	character_level INT not null

);



create table UserCharacterSlot(
	id varchar(20) not null primary key,
	character_code varchar(20) not null,
	character_slotnum int not null,

	FOREIGN KEY(id)REFERENCES User(id),
	FOREIGN KEY(character_code)REFERENCES UserCharacterInfo(character_code)
);

use userinfo;

create table SpawnPos
(
	num int not null primary key auto_increment,
	x float not null,
	y float not null,
	z float not null
);

use userinfo;

create table CharacterInfo
(
	character_jobcode INT NOT NULL primary key,
    character_jobname VARCHAR(20) NOT NULL,
    character_str INT NOT NULL,
    character_dex INT NOT NULL,
    character_int INT NOT NULL,
    character_health INT NOT NULL,
    character_mana INT NOT NULL,
    character_attackpoint INT,
    character_defensepoint INT,
    character_speed INT NOT NULL,
    character_growhealth INT NOT NULL,
    character_growmana INT NOT NULL,
    character_growstr INT NOT NULL,
    character_growdex INT NOT NULL,
    character_growint INT NOT NULL
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

create table CharacterPos
(
	character_code VARCHAR(20) NOT NULL PRIMARY KEY,
	character_pos_x FLOAT,
	chatacter_pos_y FLOAT,
	character_pos_z FLOAT,
    FOREIGN KEY(character_code) REFERENCES UserCharacterInfo(character_code)
);

use userinfo;

INSERT INTO CharacterInfo VALUES (1000,'Tanker',15,10,10,150,100,0,0,100,10,5,5,100,50),
								 (2000,'Warrior',10,15,10,100,100,0,0,100,5,10,5,50,50),
								 (3000,'Magician',10,10,15,100,150,0,0,100,5,5,10,50,100),
                                 (4000,'Gunner',10,20,5,100,100,0,0,100,5,10,5,50,50);
use userinfo;

INSERT INTO ItemInfo VALUES (1,'HealthPotion',1,1,1,'체력을 50 회복해준다',4),
                            (2,'Axe',2,1,2,'탱커의 기본무기',1),
                            (3,'Sword',2,1,2,'전사의 기본무기',2),
                            (4,'Staff',2,1,2,'마법사의 기본무기',3),
                            (5,'ClothAmor',3,1,3,'기본 천갑옷',4);
                            
INSERT INTO SpawnPos( x,y,z) VALUES (-3750.0,28330.0,6848.0),
									(-3060.0,28330.0,6848.0),
									(-2380.0,28330.0,6848.0),
									(-1690.0,28330.0,6848.0),
									(-930.0,28330.0,6848.0),
									(-160.0,28330.0,6848.0)
                    
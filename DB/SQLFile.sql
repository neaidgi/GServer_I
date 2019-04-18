create database userinfo;

use userinfo;

create table User(
	
	id varchar(20) not null primary key,

	pw varchar(20) not null
);

create table UserCharacterInfo(

	character_code varchar(30) not null primary key,
	
	id varchar(20) not null, 

	character_origin_code INT not null,

	character_jobname VARCHAR(20) not null,

	character_nickname VARCHAR(20) not null,

	character_level INT not null,
    
	character_slotnum int not null,
    
	FOREIGN KEY(id)REFERENCES User(id)
);

use userinfo;

create table SpawnPos
(
	num int not null primary key auto_increment,
	x float not null,
	y float not null,
	z float not null
);

create table DungeonSpawnPos
(
	num int not null primary key auto_increment,
	x float not null,
	y float not null,
	z float not null
);
use userinfo;

create table CharacterOrigin
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
	character_pos_y FLOAT,
	character_pos_z FLOAT,
    FOREIGN KEY(character_code) REFERENCES UserCharacterInfo(character_code)
);

create table Monsterinfo
(
	monster_code int NOT NULL PRIMARY KEY,
	monster_name VARCHAR(20),
    monster_health INT NOT NULL,
	monster_mana INT NOT NULL,
	monster_attackpoint INT,
    monster_defensepoint INT
    
);

create table MonsterSpawnPos
(
	monster_spawn_num INT NOT NULL PRIMARY KEY,
	monster_spawn_pos_x FLOAT,
	monster_spawn_pos_y FLOAT,
	monster_spawn_pos_z FLOAT
);

create table DungeonStageSpawnpos
(
	stage_spawn_num INT NOT NULL PRIMARY KEY,
	stage_spawn_pos_x FLOAT,
	stage_spawn_pos_y FLOAT,
	stage_spawn_pos_z FLOAT
);

use userinfo;

INSERT INTO CharacterOrigin VALUES (1000,'Tanker',15,10,10,150,100,0,0,100,10,5,5,100,50),
								 (2000,'Warrior',10,15,10,100,100,0,0,100,5,10,5,50,50),
								 (3000,'Magician',10,10,15,100,150,0,0,100,5,5,10,50,100),
                                 (4000,'Gunner',10,20,5,100,100,0,0,100,5,10,5,50,50);
use userinfo;

INSERT INTO ItemInfo VALUES (1,'HealthPotion',1,1,1,'체력을 50 회복해준다',4),
                            (2,'Axe',2,1,2,'탱커의 기본무기',1),
                            (3,'Sword',2,1,2,'전사의 기본무기',2),
                            (4,'Staff',2,1,2,'마법사의 기본무기',3),
                            (5,'ClothAmor',3,1,3,'기본 천갑옷',4);
                            
INSERT INTO SpawnPos( x,y,z) VALUES (162.0,28408.0,6870.0),
									(-619.0,28330.0,6870.0),
									(-1400.0,28330.0,6870.0),
									(-2381.0,28330.0,6870.0),
									(-2962.0,28330.0,6870.0),
									(-3743.0,28330.0,6870.0);
                                    
INSERT INTO dungeonspawnpos( x,y,z) VALUES	(-3407.0,23905.0,6870.0),
											(-3007.0,23905.0,6870.0),
											(-2607.0,23905.0,6870.0),
											(-2207.0,23905.0,6870.0);
                                            
INSERT INTO DungeonStageSpawnpos VALUES	(1,4690.0,18890.0,6389.0),
										(2,4387.0,19442.0,6389.0),
										(3,4381.0,18360.0,6389.0),
										(4,5325.0,18888.0,6389.0);
                                         
INSERT INTO Monsterinfo VALUES 	(10000,'Spider',100,100,10,10),
								(10001,'Warm',100,100,10,10),
                                (10002,'BossSpider',100,100,10,10);
                                
INSERT INTO MonsterSpawnPos VALUES	(1,4892.0,15000.0,6451.0),
									(2,4844.0,22780.0,6451.0),
									(3,8584.0,18840.0,6090.0)
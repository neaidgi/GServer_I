create table UserCharacterInfo(
	character_code varchar(20) not null primary key,

	character_origin_code INT not null,

	character_jobname VARCHAR(20) not null,

	character_nickname VARCHAR(20) not null,

	character_level INT not null

);


create table User(
	
	id varchar(20) not null primary key,

	pw varchar(20) not null,
);
create table UserCharacterSlot(
	id varchar(20) not null primary key,
	character_code varchar(20) not null,
	character_slotnum int not null,

	FOREIGN KEY(id)REFERENCES User(id)
	FOREIGN KEY(character_code)REFERENCES UserCharacterInfo(character_code)
);
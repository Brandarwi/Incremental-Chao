#pragma once

//#include "..\SA2programming\SA2Structs.h"//temp for CHAO_SAVE_INFO

namespace shadd {

#define DataRef(TYPE, NAME, ADDR)	  inline TYPE&	     NAME = *(TYPE*)ADDR
#define DataPtr(TYPE, NAME, ADDR)	  inline TYPE* const NAME =  (TYPE*)ADDR
#define DataAry(TYPE, NAME, ADDR, NB) typedef TYPE NAME##_t NB; DataRef(NAME##_t, NAME, ADDR)

	typedef char achar;
	typedef signed __int32 sint32;
	typedef unsigned __int32 uint32;
	typedef signed __int16 sint16;
	typedef unsigned __int16 uint16;
	typedef signed __int8 sint8;
	typedef unsigned __int8 uint8;
	typedef float float32;
	typedef signed __int32 Bool;

	struct CHAO_ID
	{
		uint32 gid[2];
		uint32 id[2];
		uint32 num;
	};

	struct AL_BODY_INFO
	{
		float32 HPos;			// Run : Power Align
		float32 VPos;			// Swim : Fly Align
		float32 APos;			// Dark : Hero Align
		float32 AimHPos;		// Last Run : Power
		float32 AimVPos;		// Last Swim : Fly
		float32 AimAPos;		// Last Alignment
		float32 growth;		// Magitude
		float32 EyePos;
		float32 EyeScl;
		float32 EyeRatio;
		uint8 EyeColor;
		uint8 DefaultEyeNum;
		uint8 DefaultMouthNum;
		uint8 HonbuNum;		// Icon kind
		uint8 HonbuColorNum;	// Inert
		uint8 ObakeHead;		// Hat / Mask
		uint8 ObakeBody;		// Hide Feet
		uint8 MedalNum;
		uint8 ColorNum;
		uint8 NonTex;			// isMonotone
		uint8 JewelNum;
		uint8 MultiNum;		// Shiny
		sint8 EggColor;
		uint8 FormNum;
		uint8 FormSubNum;		// Animal
		uint8 UNDEF0;
	};

	struct AL_TIME
	{
		uint8 minute;
		uint8 second;
		uint8 frame;
	};

	struct AL_RACE_PERSONAL_INFO
	{
		AL_TIME PersonalRecord[10];
		uint8 nbWin[10];
		uint16 MedalFlag;
	};

	struct AL_KARATE_PERSONAL_INFO
	{
		uint8 rank;
		uint8 level;
		uint8 tournament;
		uint8 UNDEF0;
		uint16 nbBattle;
		uint16 nbWin;
		uint16 nbLose;
		uint16 nbDraw;
	};

	struct AL_PARTS
	{
		uint32 MinimalFlag;
		sint8 MinimalParts[8];
	};

	struct AL_EMOTION
	{
		uint16 Flag;
		uint16 MoodTimer;
		uint16 IllTimer;
		uint16 Timer;
		uint8 Mood[8];
		uint16 State[11];
		sint8 Personality[13];
		uint8 Taste;
		uint8 Tv;
		uint8 Music;
		sint8 IllState[6];
	};

	struct AL_KNOWLEDGE_PLAYER
	{
		sint8 like;
		uint8 fear;
		uint16 distance;
		uint16 meet;
	};

	struct AL_KNOWLEDGE_CHAO
	{
		CHAO_ID id;
		sint8 like;
		uint8 fear;
		uint16 distance;
		uint16 meet;
		uint8 UNDEF0;
		uint8 UNDEF1;
	};

	struct AL_KNOWLEDGE_OTHER
	{
		sint8 like;
	};

	struct AL_KNOWLEDGE_BTL
	{
		uint8 ArtFlag;
		uint8 DanceFlag;
		uint8 SongFlag;
		uint8 MusicFlag;
		uint16 SToyFlag;
		uint16 LToyFlag;
		sint32 KwTimer;
		AL_KNOWLEDGE_PLAYER player[6];
		AL_KNOWLEDGE_CHAO chao[20];
		AL_KNOWLEDGE_OTHER bhv[120];
	};

	struct AL_GENE
	{
		sint8 IsAnalyzed;
		sint8 EggColor;
		uint8 UNDEF0;
		uint8 UNDEF1;
		CHAO_ID MotherID;
		CHAO_ID FatherID;
		achar MotherName[8];
		achar FatherName[8];
		achar MGroundMotherName[8];
		achar MGroundFatherName[8];
		achar FGroundMotherName[8];
		achar FGroundFatherName[8];
		uint8 Abl[8][2];
		uint8 LifeTime[2];
		sint8 HPos[2];
		sint8 VPos[2];
		sint8 APos[2];
		sint8 Personality[13][2];
		uint8 Taste[2];
		uint8 Tv[2];
		uint8 Music[2];
		uint8 Color[2];
		uint8 NonTex[2];	// Monotone
		uint8 Jewel[2];
		uint8 Multi[2];	// Shiny
		uint8 EyePos[2];
		uint8 EyeScl[2];
		uint8 EyeRatio[2];
		uint8 EyeColor[2];
	};

	struct AL_RACE_PERSONAL_INFO_DX
	{
		AL_TIME PersonalRecord[5];
	};

	struct AL_KNOWLEDGE_DX
	{
		AL_KNOWLEDGE_PLAYER player[6];
	};

	struct CHAO_PARAM_GC
	{
		uint8 GBAChao;
		uint8 GBAEgg;
		uint8 GBABerry[8];
		uint8 padding0;
		uint8 padding1;
		uint32 GBARing;				// Rings moving from GBA -> GCN
		uint8 BootMethed;
		uint8 Birthplace;
		uint8 name[7];
		uint8 GBAType;
		uint8 GBASkin;
		uint8 GBAMood;
		uint8 GBABelly;
		uint8 GBASleepy;
		uint8 GBALonelyness;
		uint8 padding2;
		uint8 Exp[8];				// Progress 0 - 100
		uint8 Abl[8];				// Grade E - S
		uint8 Lev[8];				// Level 0 - 99
		uint16 Skill[8];			// Total Points
		uint16 GBAPallete[16];
		uint8 rmsg[16];
		uint32 runaway;
		uint8 dummy[4];
		uint8 type;					// Chao Type
		uint8 place;				// Chao Area
		sint16 like;				// Happiness
		sint8 ClassNum;
		uint8 UNDEF0;
		uint16 age;					// total clock rollovers
		uint16 old;					// adult clock rollovers
		uint16 life;				// Liftime1
		uint16 LifeMax;				// Liftime2
		uint16 nbSucceed;			// reincarnations
		CHAO_ID ChaoID;
		uint32 LifeTimer;
		AL_BODY_INFO body;
		AL_RACE_PERSONAL_INFO race;
		AL_KARATE_PERSONAL_INFO karate;
		uint8 UNDEF1;
		uint8 UNDEF2;
		AL_PARTS PartsBTL;
		AL_EMOTION emotion;
		AL_KNOWLEDGE_BTL knowledge;
		AL_GENE gene;
		Bool IsInitializedDX;
		AL_PARTS partsDX;
		AL_RACE_PERSONAL_INFO_DX raceDX;
		uint8 UNDEF3;
		AL_KNOWLEDGE_DX knowledgeDX;
	};

	struct chao_save_info
	{
		CHAO_PARAM_GC param;
		uint8 dummy[736];
	};

	struct AL_SHAPE_ELEMENT
	{
		uint8 type;
		uint8 DefaultEyeNum;
		uint8 DefaultMouthNum;
		uint8 HonbuNum;
		uint8 ObakeHead;
		uint8 ObakeBody;
		uint8 MedalNum;
		uint8 ColorNum;
		uint8 NonTex;
		uint8 JewelNum;
		uint8 MultiNum;
		sint8 MinimalParts[8];
		sint8 UNDEF0;
		sint16 HPos;
		sint16 VPos;
		sint16 APos;
		sint16 Growth;
		uint8 name[8];
		uint16 Skill[8];
	};

	struct AL_RECORD
	{
		sint8 RecordFlag;
		AL_TIME time;
		AL_SHAPE_ELEMENT ShapeElement;
	};

	struct AL_RACE_INFO
	{
		sint8 RaceActiveFlag[6];
		sint8 CourseChallengedLevel[13];
		sint8 BeginnerClearedLevel[4];
		sint8 JewelClearedLevel[6];
		sint8 ChallengeClearedLevel;
		sint8 HeroClearedLevel;
		sint8 DarkClearedLevel;
		AL_RECORD CourseRecord[10];
		AL_TIME time;
		sint8 CurrRaceNum;
		sint8 CurrCourseNum;
		sint8 CurrLevelNum;
		sint8 nbUserEntry;
		CHAO_SAVE_INFO ChaoInfo[8];
	};

	DataRef(AL_RACE_INFO, RaceInfo, 0x01A0F980);
	//DataRef(Bool, chaoWorldIsLoaded, 0x01A5AF0C);

	//Brandarwi
	DataRef(sint32, chaoWorldIsLoaded, 0x01A5AF0C);//signed __int32& cwil = *(signed __int32*)0x01A5AF0C;
	//Brandarwi
}
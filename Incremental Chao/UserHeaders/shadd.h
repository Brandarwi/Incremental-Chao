#pragma once

#include "..\SA2programming\SA2Structs.h"//temp for CHAO_SAVE_INFO

#define DataRef(TYPE, NAME, ADDR)	  inline TYPE&	     NAME = *(TYPE*)ADDR
#define DataPtr(TYPE, NAME, ADDR)	  inline TYPE* const NAME =  (TYPE*)ADDR
#define DataAry(TYPE, NAME, ADDR, NB) typedef TYPE NAME##_t NB; DataRef(NAME##_t, NAME, ADDR)

typedef signed __int32 sint32;
typedef unsigned __int32 uint32;
typedef signed __int16 sint16;
typedef unsigned __int16 uint16;
typedef signed __int8 sint8;
typedef unsigned __int8 uint8;

//Brandarwi
//typedef signed __int32 Bool;
//

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

struct SHADD_AL_TIME
{
	uint8 minute;
	uint8 second;
	uint8 frame;
};

struct AL_RECORD
{
	sint8 RecordFlag;
	SHADD_AL_TIME time;
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
	SHADD_AL_TIME time;
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
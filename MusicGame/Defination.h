#pragma once
#include "SSD1306Wire.h"
#define SongArrayLength  4
SSD1306Wire display(0x3c, SDA, SCL); 
int Channel = 0;
int Res = 8;
int fqz  =2000;
int BuzzerPin =25;

class StartStateDefine{   

public:

   String WeclomeText = String( "**Music Game**");
   const uint8_t* WeclomeTextFont = ArialMT_Plain_16;
   int16_t WeclomeText_X = 0;
   int16_t WeclomeText_Y = 0;



   String SubText = String("Main Menu show After 5s!");
   const uint8_t* SubTextFont = ArialMT_Plain_10;
   int16_t SubText_X = 0;
   int16_t SubText_Y = 27;
   unsigned long StartStateOverMillSec = 5000;

};


class MainMenuStateDefine{

public:

    int16_t SongArray = SongArrayLength ;
    String SongName[SongArrayLength] = {

    
    String("Sky Castle") ,  
    String("demon slayer") ,
    String("Senbon Sakura"),
    String("YOASOBI Tabun"),



    };

    const uint8_t* SongNameFont = ArialMT_Plain_16;
    int16_t SongNameTextStart_X = 0;
    int16_t SongNameTextStart_Y = 0; 




    /*選擇鼠標*/

    int16_t Cursor_X = 120;
    int16_t Cursor_Radius = 8;
    int16_t Cursor_offest_Y = 16;


};


class MusicStateDefine{


public:

    int16_t SplitLine_X = 0;
    int16_t SplitLine_Y = 64;
    int16_t SplitLine_Lenght = 128;

};


class IAphlaBatPointDefine{

public:

    float Aphlabat_radius = 6.25;
    float OffestSpeed = 2;
    float OffestDelay = 14.0f;
    int16_t HitPos = 20 ;
    float HitPosTolirance = 1.85f;
    int16_t HitPosStart_X = 128;
    int16_t HitPosStart_Y = 32;

    float Score = 100;
    float Score_Prefect = 2.0f;
    float Score_Good  = 1.5f;
    float Score_Normal = 1.0f;
    float Score_Bad = 0.7f;
    

};


class ClaCulateStateDefine{

public:
    String Text = String("** Final Score **");
   const uint8_t* TextFont = ArialMT_Plain_24;
   int16_t Text_X = 0;
   int16_t Text_Y = 0;



   const uint8_t* SubTextFont = ArialMT_Plain_10;
   int16_t SubText_X = 20;
   int16_t SubText_Y = 0;
   unsigned long StartStateOverMillSec = 5000;
    
};

static StartStateDefine _StartStateDefine; 
static MainMenuStateDefine _MainMenuStateDefine;
static MusicStateDefine _MusicStateDefine;
static IAphlaBatPointDefine _IAphlaBatPointDefine;
static ClaCulateStateDefine _ClaCulateStateDefine;
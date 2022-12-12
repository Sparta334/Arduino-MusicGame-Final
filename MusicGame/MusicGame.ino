#include "Defination.h"
#include "Song.h"
#include "SSD1306Wire.h"
#include "Vector.h"
#include "BluetoothSerial.h"
#include "driver/gpio.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define	IMPLEMENTATION	LIFO
#define BUZZERPIN GPIO_NUM_25

class StateController;
class IState;
class IAphlaBatPoint;


BluetoothSerial BT;

float CurrentShakeTime  = 0;
float LastShakeTime  = 0;
  bool IsPrssppL = false;
  bool IsPrssppR = false;
/* 基本場景宣告*/

class IAphlaBatPoint{
  
  


public:

float OffestCurrentTime = 0;
  float OffestLastTime = 0  ;
  float _PointX;
  float _PointY;

   bool Prefect = false;
  bool Good = false;
  bool Normal = false;
  bool Bad = false;
  bool Miss = false;
  


  virtual void draw(){}
  virtual float Hit( ){
      
      Prefect = false;
      Good = false;
      Normal = false;
      Bad = false;
      Miss = false;
      float score = 0;
 
    if(_PointX > _IAphlaBatPointDefine.HitPos- (_IAphlaBatPointDefine.Aphlabat_radius*4)  &&_PointX < _IAphlaBatPointDefine.HitPos+ (_IAphlaBatPointDefine.Aphlabat_radius*6)   ){
   
        score += _IAphlaBatPointDefine.Score;
          if( _PointX <= _IAphlaBatPointDefine.HitPos + _IAphlaBatPointDefine.HitPosTolirance*2 &&  _PointX >= _IAphlaBatPointDefine.HitPos - _IAphlaBatPointDefine.HitPosTolirance*2  ){

                score *= _IAphlaBatPointDefine.Score_Prefect;
              /*Perfect 的範圍 */
              Prefect =true;
          }
          else if( _PointX <= _IAphlaBatPointDefine.HitPos + (_IAphlaBatPointDefine.HitPosTolirance*4) &&  _PointX >= _IAphlaBatPointDefine.HitPos - (_IAphlaBatPointDefine.HitPosTolirance*4)  ){

            /*Good 的範圍 */
            score *= _IAphlaBatPointDefine.Score_Good;
            Good = true;

          }
          else if(_PointX <= _IAphlaBatPointDefine.HitPos + (_IAphlaBatPointDefine.HitPosTolirance*6 ) &&  _PointX >=_IAphlaBatPointDefine.HitPos - (_IAphlaBatPointDefine.HitPosTolirance*6) ){
            /*Normal 的範圍 */

            score *= _IAphlaBatPointDefine.Score_Normal;

            Normal = true;
          }
          else if(_PointX <=  _IAphlaBatPointDefine.HitPos + (_IAphlaBatPointDefine.HitPosTolirance*8 ) &&  _PointX >= _IAphlaBatPointDefine.HitPos - (_IAphlaBatPointDefine.HitPosTolirance*8 )){
            /*bad 的範圍 */

            score *= _IAphlaBatPointDefine.Score_Bad;
            Bad  = true;
          }
          else{
            score= 1;
            Miss =true;
          }
   

    }
    else{
       
       score = 0;
    }
  


    return score;

  }
  virtual float offest(byte cmd ){ return 0;}
  virtual void offest( ){ 
     OffestCurrentTime = millis();
     if(OffestCurrentTime - OffestLastTime >= _IAphlaBatPointDefine.OffestDelay ){

          this->_PointX-=_IAphlaBatPointDefine.OffestSpeed;
          OffestLastTime = OffestCurrentTime;
     }
  }
  IAphlaBatPoint(float PointX , float PointY){_PointX = PointX ; _PointY  = PointY; }
  IAphlaBatPoint(){_PointX = 128 ; _PointY  =32;}


};



/*實心大音符*/
class FillCircle : public IAphlaBatPoint{


public:
  FillCircle(float PointX , float PointY) : IAphlaBatPoint(PointX , PointY){
    
  }

   
  float offest(byte cmd ) override {
    float Temp = 0;
    OffestCurrentTime = millis();
     if(OffestCurrentTime - OffestLastTime >= _IAphlaBatPointDefine.OffestDelay ){

          this->_PointX-=_IAphlaBatPointDefine.OffestSpeed;
          OffestLastTime = OffestCurrentTime;
     }

    if(CurrentShakeTime - LastShakeTime >= 14.0f){
      if(cmd == 4 && (!IsPrssppL)){

          Serial.println("Fill");
          Temp = this->Hit();

            
          IsPrssppL = true;
          LastShakeTime = CurrentShakeTime;

      }


    }

      if(  cmd ==6 && (IsPrssppL)){

          IsPrssppL = false;

      }

     

    return Temp;

  }


   virtual void draw() override {

    display.fillCircle( _PointX , _PointY , _IAphlaBatPointDefine.Aphlabat_radius);

  }

};


class HollowCircle: public IAphlaBatPoint{

public:

  HollowCircle(float PointX , float PointY) : IAphlaBatPoint(PointX , PointY){

  }


  float offest(byte cmd  ) override {

     float Temp = 0;
      OffestCurrentTime = millis();
     if(OffestCurrentTime - OffestLastTime >= _IAphlaBatPointDefine.OffestDelay ){

          this->_PointX-=_IAphlaBatPointDefine.OffestSpeed;
          OffestLastTime = OffestCurrentTime;
     }

 if(CurrentShakeTime - LastShakeTime >= 14.0f){
    if( cmd == 5 && (!IsPrssppR)){
      Serial.println("Hollow");
       Temp = this->Hit();

        IsPrssppR = true;
        LastShakeTime = CurrentShakeTime;

    }
   

    
 }

  if(  cmd ==6 && (IsPrssppR)){

        IsPrssppR = false;

  }

    return Temp;

  }

  virtual void draw() override {

    display.drawCircle( _PointX , _PointY , _IAphlaBatPointDefine.Aphlabat_radius);

  }

};




class FillComboHit : public IAphlaBatPoint{


public:
  FillComboHit(float PointX , float PointY) : IAphlaBatPoint(PointX , PointY){
    
  }

  float Hit(  ){
      float score = 0;
      if(_PointX <= _IAphlaBatPointDefine.HitPos + (_IAphlaBatPointDefine.HitPosTolirance*4) &&  _PointX >= _IAphlaBatPointDefine.HitPos - (_IAphlaBatPointDefine.HitPosTolirance*4)  ){

        score += _IAphlaBatPointDefine.Score*_IAphlaBatPointDefine.Score_Good;
        Prefect = true;

      }
      else{
        
        score = 0;
      }

      return score;

  }
  float offest(byte cmd) override {
        float Score = 0; 
      OffestCurrentTime = millis();
     if(OffestCurrentTime - OffestLastTime >= _IAphlaBatPointDefine.OffestDelay ){

          this->_PointX-=_IAphlaBatPointDefine.OffestSpeed;
          OffestLastTime = OffestCurrentTime;
     }

    if(  cmd ==4 && (!IsPrssppR)){

        Score+=this->Hit();
        IsPrssppR = true;

    }
    if( cmd ==6 && (IsPrssppR)){

        IsPrssppR = false;

    }

    if( cmd ==5 && (!IsPrssppL)){

        Score+=this->Hit();
        IsPrssppL = true;

    }
    if( cmd ==6 && (IsPrssppL)){

        IsPrssppL = false;

    }
    
    return Score;

  }


  void draw() override {

    display.fillCircle( _PointX , _PointY , _IAphlaBatPointDefine.Aphlabat_radius*2.3f);

  }

};

class None : public IAphlaBatPoint{


public:
  None(float PointX , float PointY) : IAphlaBatPoint(PointX , PointY){
    
  }

  
  float offest(byte cmd ) override {

      OffestCurrentTime = millis();
     if(OffestCurrentTime - OffestLastTime >= _IAphlaBatPointDefine.OffestDelay ){

          this->_PointX-=_IAphlaBatPointDefine.OffestSpeed;
          OffestLastTime = OffestCurrentTime;
     }

    return false;

  }


  void draw() override{};

};



  Vector<IAphlaBatPoint*> V0  ;

class IState{

public: 
  String _state_name = String("NULL");
  

protected:

  
  StateController* _stateController = nullptr;

public:
  virtual ~IState(){}
  IState(StateController* stateController );   /*傳入StateController                                                                   這樣才能藉由Controller
                                                                                       更新State 
                                                                                        */

  virtual void StateBegin(){}
  virtual void StateUpdate(){}
  virtual void StateEnd(){}
};


/* Start場景宣告定義*/

class StartState :public IState {

  
public:

  /*屬性宣告*/
  unsigned long CurrentTime;
  unsigned long LastTime;
  
  

  /*繼承覆寫*/
  
  virtual void StateBegin() override {
    CurrentTime = millis();
    LastTime = CurrentTime ;

    display.clear();

    display.setFont(  _StartStateDefine.WeclomeTextFont );
    display.drawString(  _StartStateDefine.WeclomeText_X , _StartStateDefine.WeclomeText_Y, _StartStateDefine.WeclomeText    );
    display.setFont(  _StartStateDefine.SubTextFont );
    display.drawString(  _StartStateDefine.SubText_X , _StartStateDefine.SubText_Y, _StartStateDefine.SubText    );
    display.display();

    



  }

   virtual void StateUpdate() override ;
  
  
   virtual void StateEnd() override ;


  /*自己的method*/

  /*建構*/
  StartState( StateController* stateController ):IState(stateController){

    _state_name = "StartState" ;
    Serial.println("Change!!");

  }

  ~StartState(){
  }


  
};

/* MainMenuState場景宣告定義*/

class MainMenuState :public IState {

public:

  /*屬性宣告*/

  int _index;
  int16_t cursor_pos_Y ; 
  bool IsPressR ;
  bool IsPressL;
  int Page ;
  int temp_i = 0;

  /*繼承覆寫*/
  
  virtual void StateBegin() override {


    _index = 0;
    cursor_pos_Y = 0 ;

    display.clear();
    IsPressR = false;
    IsPressL = false;
    Page = 1;


    
  }

   virtual void StateUpdate()  override;

   
      
   virtual void StateEnd() override ;





  /*自己的method*/
   MainMenuState( StateController* stateController ):IState(stateController){

    _state_name = "MainMenuState" ;
    Serial.println("Change!!");

  }

  ~MainMenuState(){}

private:

  int BtnChoice(int  cmd , int index) {


        if( cmd == 2  ){
            
            index+=1;
            if(index >= _MainMenuStateDefine.SongArray ){
              index = 0;
            }

           

        }




        if( cmd == 1 ){

            index-=1;
            if(index < 0 ){
              index =  _MainMenuStateDefine.SongArray-1;
            }
        }

     
    Page = (index /4) +1;
    return index;

  }

  void CursorOffset(){

    cursor_pos_Y = (_index%4)* _MainMenuStateDefine.Cursor_offest_Y+2 ;

  }


  void Draw(){

      display.clear();

      display.setFont( _MainMenuStateDefine.SongNameFont );
      
      for(int i = 0  ; i < SongArrayLength ; i+=1){

          if( Page > 1 ){

              if( i < (Page-1)*4 || i >= (Page)*4  ){
                continue;
              }

          }
          else{

              if(i>4){
                continue;
              }
          }

            
          display.drawString( _MainMenuStateDefine.SongNameTextStart_X , i * _MainMenuStateDefine.Cursor_offest_Y   , _MainMenuStateDefine.SongName[i] );
          

      }
      
      display.fillCircle( _MainMenuStateDefine.Cursor_X , cursor_pos_Y+6 , _MainMenuStateDefine.Cursor_Radius  );
      
      display.display();
  }

};
  IAphlaBatPoint Pont  ;

class MusicState :public IState {

public:

  /*屬性宣告*/


  int _index;
 
  float CurrentTime;
  float LastTime;
  float CurrentTimeHit;
  float LastTimeHit;
  float TextTimeCurrent;
  float TextTimeLast;

  float _SongCurrentTime ;
  float _SongLastTime ;
  
  unsigned long _SongStartTime;

  unsigned long _SongIndex ;
  unsigned long _SongIndexBuzzer;
  unsigned long _SongLength;
  unsigned long _SongBPM;
  String _HitPoint;
  bool Prefect = false;
  bool Good = false;
  bool Normal = false;
  bool Bad = false;
  bool Miss = false;
  float IsTemp ;

  float Score;

  unsigned long MusicIndex;
  bool PlayB = true;
  byte cmd =-1;

  
  /*繼承覆寫*/


  virtual  void StateBegin() override {

    _SongBPM = SongBPM[_index];
    _SongLength =SongLength[_index];
    Serial.println(_SongLength);
    _HitPoint = HitPoint[_index];


    CurrentTime = millis();
    CurrentTimeHit = millis();
    _SongCurrentTime = millis();
    TextTimeCurrent = millis();

    LastTime = CurrentTime;
    LastTimeHit = CurrentTimeHit;
    _SongLastTime =  _SongCurrentTime ;
    TextTimeLast= TextTimeCurrent;
    
    _SongStartTime = SongStartTime[_index];
    Score = 0;
    _SongIndex  = 0;
    _SongIndexBuzzer = 0;

    V0.Clear();
    Serial.println(ceil(4900.0f/_SongBPM));



  } 

   virtual void StateUpdate()  override;

   
      
   virtual void StateEnd() override ;



  /*自己的method*/
  MusicState( StateController* stateController , int index ):IState(stateController){

    _state_name = "MusicState" ;
    _index = index;
    Serial.println("Change!!");

  }
  ~MusicState(){}
  
  void PlayMusic(){

      if(CurrentTime - LastTime >= SongBGM_Delay[_index][_SongIndexBuzzer]   ){
        
        ledcWriteTone(Channel, SongBGM[_index][_SongIndexBuzzer]); 

          _SongIndexBuzzer+=1;
      }

  }

  
  void DrawBack(){
    display.drawVerticalLine(_IAphlaBatPointDefine.HitPos , 0 , 62);
  }

  void SpawnHitPoint(){


    CurrentTimeHit = millis();

    if(CurrentTimeHit  - LastTimeHit  >=   (5000.0f/_SongBPM)){

      IAphlaBatPoint*  Temp =   String2Aphlabat (_HitPoint);

      if( Temp != nullptr)
        V0.PushBack( Temp );

      _SongIndex+=1;

      LastTimeHit = CurrentTimeHit;
   }
    

  }


    void Destroy(int i){
      V0.Erase(i );
   
  }

  void Draw(){

      
      IsTemp = 0;
      
      SpawnHitPoint();

      if(V0.Size()>0){
          IsTemp = V0[0]->offest(cmd );
      }

      for(int i = 1 ; i <V0.Size(); i++){

         V0[i]->offest();
          
      }


      for(int i = 0; i < V0.Size();i++){
        V0[i]->draw();

      }

      
      if(IsTemp>0 ){

          if(IsTemp != 1 ){
              Score+=IsTemp;
          }
          
          Pont = *(V0[0]);
          Destroy(0);

      }

       if(V0.Size()>0){
            if((V0[0]->_PointX <= 4)){

            Destroy(0);

        }
      }
      
       if(Pont.Prefect ){
              display.setFont(ArialMT_Plain_24);
              display.drawString(40 ,40 , "Perfect");

            
          }
          else if(Pont.Good){
           
               display.setFont(ArialMT_Plain_24);
              display.drawString(40 ,40 , "Good");
          }
          else if(Pont.Normal){
            
             display.setFont(ArialMT_Plain_24);
              display.drawString(40 ,40 , "Normal");

            
          }
          else if(Pont.Bad){
             display.setFont(ArialMT_Plain_24);
              display.drawString(40 ,40 , "Bad");

          }
          else if(Pont.Miss){
               display.setFont(ArialMT_Plain_24);
              display.drawString(40 ,40 , "Miss");


         }


        



       if(TextTimeCurrent - TextTimeLast > 2000){
            Pont.Bad = false;
            Pont.Miss = false;
            Pont.Normal = false;
            Pont.Good = false;
            Pont.Prefect = false;
             TextTimeLast =TextTimeCurrent;
        }
         




       
}


  IAphlaBatPoint* String2Aphlabat(String HitPointString){

      switch (HitPointString[ _SongIndex ])
      {
      case '1':
        return new FillCircle( _IAphlaBatPointDefine.HitPosStart_X , _IAphlaBatPointDefine.HitPosStart_Y  );
        break;
      case '2':
        return  new HollowCircle(_IAphlaBatPointDefine.HitPosStart_X ,_IAphlaBatPointDefine.HitPosStart_Y);
        break;
      case '8':
       return  new FillComboHit(_IAphlaBatPointDefine.HitPosStart_X ,_IAphlaBatPointDefine.HitPosStart_Y);
        break;
      case '0':
        return nullptr;
        break;
      }

     return nullptr;
      
  } 

  

};


class CalculateState :public IState {

  
public:

  /*屬性宣告*/
  unsigned long CurrentTime;
  unsigned long LastTime;
  float _Score;
   int16_t _index;
  byte cmd = -1;
  /*繼承覆寫*/
  
  virtual void StateBegin() override {
    CurrentTime = millis();
    LastTime = CurrentTime ;

  

    display.clear();

    display.setFont(  _ClaCulateStateDefine.TextFont);
    display.drawString(  _ClaCulateStateDefine.Text_X, _ClaCulateStateDefine.Text_Y, String(_Score)   );
   // display.setFont(  _StartStateDefine.SubTextFont );
    //display.drawString(  _ClaCulateStateDefine.SubText_X , _ClaCulateStateDefine.SubText_Y,   );
    display.display();

    



  }

   virtual void StateUpdate() override;

   virtual void StateEnd() override;

  /*自己的method*/

  /*建構*/
  CalculateState( StateController* stateController , float Score , int index ):IState(stateController){
    _index = index;
    _Score  = Score ;
    _state_name = "CalculateState" ;
    Serial.println("Change!!");

  }
  ~CalculateState(){}
  
};


/* 控制場景 */
class StateController{

public:
  IState* _state = nullptr;
  bool StateFirstRun = true;
  ~StateController(){}
public:

   virtual void SetState(IState* state , String state_name ){   /* 讓繼承IState的其他State做更新，換成新的State*/

    if(_state != nullptr){
      _state->StateEnd();
      delete _state;
    }

    
    Serial.println(state_name);
    _state = state;
    
  }

   virtual void StateUpdate(){

      if( _state != nullptr && StateFirstRun ){
        _state->StateBegin();
        StateFirstRun = false;
      }

    if( _state != nullptr ){
     
      _state->StateUpdate();      /* 目前的State的Update，_state.StateUpdate()等於就是
                                   遊戲每禎更新

                                 */
    }
     
  }
  
  
};

/*Game Loop*/
StateController* stateController;

void setup() {

  /*嘗試拉高電壓*/
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = 1ULL<<BUZZERPIN;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;

    gpio_config(&io_conf);

    gpio_set_level(BUZZERPIN ,1);
  /*wifi 上傳*/
  
  Serial.begin(115200);
  Serial.println("Booting");
  BT.begin("MusicGame");
  //SerialBT.begin("MusicGame");

  // put your setup code here, to run once:
   stateController = new StateController();

  ledcSetup(Channel , fqz, Res );

  ledcAttachPin(BuzzerPin,  Channel );
    
  display.init();
  display.flipScreenVertically();
  stateController->SetState(new StartState( stateController ) , "StartState"  );


}

void loop() {
  // put your main code here, to run repeatedly:

  stateController->StateUpdate();
}


void StartState::StateEnd(){

     _stateController->StateFirstRun  = true;
}

 void StartState::StateUpdate(){

   CurrentTime = millis();



    /*切換State*/

    if(CurrentTime - LastTime > _StartStateDefine.StartStateOverMillSec){
      _stateController->SetState( new MainMenuState( _stateController) , "MainMenuState" );
    }


    
}

void MainMenuState::StateEnd(){

     _stateController->StateFirstRun  = true;
}


void MainMenuState::StateUpdate(){

      byte cmd = BT.read();
      
      _index = BtnChoice(cmd , _index);
        CursorOffset();

        Draw();

       /*切換State*/
        if( cmd == 3  ){

            _stateController->SetState(new MusicState( _stateController , _index) , "MusicState"  );

        }
}

void MusicState::StateEnd(){

     _stateController->StateFirstRun  = true;
     ledcWriteTone(Channel, 0); 

}


void MusicState::StateUpdate(){

    cmd = BT.read();
    
    display.clear();
   CurrentTime = millis();
      CurrentTimeHit = millis();
      _SongCurrentTime = millis();
      TextTimeCurrent = millis();
     CurrentShakeTime = millis();
      Draw();

     if(_SongCurrentTime -  _SongLastTime >= SongStartTime[_index]){
        PlayMusic();
     }
     else{
        LastTime = CurrentTime;
     }

      DrawBack();
      
      
      display.display();
       /*切換State*/

      if(_SongIndexBuzzer  >= _SongLength){
            BT.write(0);
            _stateController->SetState(new CalculateState(_stateController,Score , _index) , "CalculateState" );
      }
       
       
}

void CalculateState::StateEnd(){

     _stateController->StateFirstRun  = true;

}


void CalculateState::StateUpdate(){
  CurrentTime = millis();
  cmd = BT.read();

    /*切換State*/

    if( cmd == 7){

      _stateController->SetState( new StartState( _stateController ) , "StartState" );

    }


}


IState::IState(StateController* stateController ) {_stateController =  stateController;}

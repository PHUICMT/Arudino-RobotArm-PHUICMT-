#include<ipst.h>
#include <PS2X_lib.h>
#define PS2_DAT 16
#define PS2_CMD 17
#define PS2_SEL 18
#define PS2_CLK 19
PS2X ps2x;

int speed=5; // Speed

const int ledPin = 20;
int grab=21; // Start 20 --> [20 - 60]
int servo_Left=1; // Start 1 --> [1 - 44]
int servo_Right=150; // Start 150 --> [180 - 100]
int servo_Center = 90; //Start 90 --> [1 - 180]

//SAVE POSITION//
int countPosition = 0;
int grab_position[20];
int servo_Left_position[20];
int servo_Right_position[20];
int servo_Center_position[20];
boolean StartLoop = false;
boolean Loop = false;
//END SAVE POSITION//

void setup()
{
  Serial.begin(9600);
  //Serial.println("Connecting");
  
  while(true){
    int error = ps2x.config_gamepad(PS2_CLK,PS2_CMD,PS2_SEL,PS2_DAT,false,false);
    if(error==0){
      //Serial.print("OK");
      //delay(1000);
      break;
    }
  }
  servo(3,20); //GRAB
  servo(2,0);  //SERVO_LEFT
  servo(1,150); //SERVO_RIGHT
  servo(0,90); // CENTER
  ResetPosition();
  pinMode(ledPin, OUTPUT); //LED
} //END SET UP

void ResetPosition(){ //ResetPosition
  countPosition = 0;
  for(int i=0;i<20;++i){
    grab_position[i] = 20;
    servo_Left_position[i] = 1;
    servo_Right_position[i] = 150;
    servo_Center_position[i] = 90;
  }

}
void SetPosition(int grab,int left,int right,int center){ //Save Position
   grab_position[countPosition] = grab;
   servo_Left_position[countPosition] = left;
   servo_Right_position[countPosition] = right;
   servo_Center_position[countPosition] = center;
   ++countPosition;
}
void Set_home(){ /// SET HOME //
  for(int i=grab;i>=20;--i){
    servo(3,i);
    delay(10); //For smooth
  }
  grab = 20;
  
  for(int i=servo_Left;i>=1;--i){
    servo(2,i);
    delay(10); //For smooth
  }
  servo_Left=1;
  
  if(servo_Left <=150){
    for(int i=servo_Right;i<=150;++i){
      servo(1,i);
      delay(10); //For smooth
    }
  }else{
    for(int i=servo_Right;i>=150;--i){
      servo(1,i);
      delay(10); //For smooth
    }
  }
  servo_Right=150; // 180 - 100
  
  if(servo_Center <= 90){
    for(int i=servo_Center;i<=90;++i){
      servo(0,i);
      delay(10); //For smooth
    }
  }else{
     for(int i=servo_Right;i>=90;--i){
      servo(0,i);
      delay(10); //For smooth
    }
  }
  servo_Center = 90; //Start 90
}/// END SET HOME ///

void StartLoopPosition(){ //StartLoopPosition////////////////////////////////////////////////////////

  for(int i=0;i<countPosition;++i){
    digitalWrite(ledPin, HIGH);
    delay(100);
    if(grab <= grab_position[i]){ //Grab
      for(int j=grab;j<=grab_position[i];++j){
        servo(3,j);
        delay(10); //For smooth
        grab = j;
      }
    }else{
      for(int j=grab;j>=grab_position[i];--j){
        servo(3,j);
        delay(10); //For smooth
        grab = j;
      }
    }//End Grab
    if(servo_Left <= servo_Left_position[i]){ //Left
      for(int j=servo_Left;j<=servo_Left_position[i];++j){
        servo(2,j);
        delay(10); //For smooth
        servo_Left = j;
      }
    }else{
      for(int j=servo_Left;j>=servo_Left_position[i];--j){
        servo(2,j);
        delay(10); //For smooth
        servo_Left = j;
      }
    } // End Left
    if(servo_Right <= servo_Right_position[i]){ //Right
      for(int j=servo_Right;j<=servo_Right_position[i];++j){
        servo(1,j);
        delay(10); //For smooth
        servo_Right = j;
      }
    }else{
      for(int j=servo_Right;j>=servo_Right_position[i];--j){
        servo(1,j);
        delay(10); //For smooth
        servo_Right = j;
      }
    }//END Right
    if(servo_Center <= servo_Center_position[i]){ //CENTER
    delay(100);
      for(int j=servo_Center;j<=servo_Center_position[i];++j){
        servo(0,j);
        delay(10); //For smooth
        servo_Center = j;
      }
    }else{
      for(int j=servo_Center;j>=servo_Center_position[i];--j){
        servo(0,j);
        delay(10); //For smooth
        servo_Center = j;
      }
    }
   //END Center
  }
  digitalWrite(ledPin, LOW);
} // END StartLoopPosition ///////////////////////////////////////////////////////////////////
int Run(int x){
  ps2x.read_gamepad(false,false);
  if(ps2x.Button(PSB_PAD_UP) ||  ps2x.Button(PSB_PAD_LEFT)){
    //glcd(1,1.5,"UP");
    if(ps2x.Button(PSB_L2)){
     x+=speed; 
    }else{
      x+=2;
    }
    //glcdClear();
    
  }else if(ps2x.Button(PSB_PAD_DOWN) || ps2x.Button(PSB_PAD_RIGHT)){
    //glcd(3,1.5,"DOWN");
    if(ps2x.Button(PSB_L2)){
     x-=speed; 
    }else{
      x-=2;
    }
    //glcdClear();
  }
  return x;
}
void loop() //////////////////////////////////////////////////////////////////////////////////////////////////////
{
  ps2x.read_gamepad(false,false);
  if(ps2x.Button(PSB_R1)){ //Start Work Loop
    if(countPosition != 0 ){
      for(;;){
        StartLoopPosition(); 
      }
    }
  }
  if(ps2x.Button(PSB_TRIANGLE)  ){ //GRAB
    //glcd(2,1.5,"GRAB"); 
    if(ps2x.Button(PSB_PAD_LEFT)){
      grab = 60;
      servo(3,grab); 
    }else if(ps2x.Button(PSB_PAD_RIGHT)){
      grab = 20;
      servo(3,grab);
    } 
    digitalWrite(ledPin, HIGH);
    delay(100);
    //delay(1000);//END GRAB
    
  }else if(ps2x.Button(PSB_SQUARE)){ //LEFT = DOWN!!
    //glcd(2,1.5,"DOWN");
    if(servo_Left > 0 && servo_Left < 45){
      servo_Left = Run(servo_Left);
      if(servo_Left < 1){
        servo_Left = 1;
      }else if(servo_Left > 44){
        servo_Left = 44;
      }
    }
    servo(2,servo_Left); //END DOWN
    digitalWrite(ledPin, HIGH);
    
  }else if(ps2x.Button(PSB_CIRCLE)){ //RIGHT  180 - 100
    //glcd(2,1.5,"RIGHT");
    if(servo_Right > 100 && servo_Right < 180){
      if(ps2x.Button(PSB_PAD_DOWN) || ps2x.Button(PSB_PAD_RIGHT)){
        if(ps2x.Button(PSB_L2)){
         servo_Right-=speed; 
        }else{
          servo_Right-=2;
        }
      }else if(ps2x.Button(PSB_PAD_UP) ||  ps2x.Button(PSB_PAD_LEFT) ){
        if(ps2x.Button(PSB_L2)){
         servo_Right+=speed; 
        }else{
         servo_Right+=2;
        }
      }
      if(servo_Right < 101){
        servo_Right = 101;
      }else if(servo_Right > 179){
        servo_Right = 179;
      }
    }
    servo(1,servo_Right); //END RIGHT
    digitalWrite(ledPin, HIGH);
  }else if(ps2x.Button(PSB_CROSS)){ //CENTER
    //glcd(2,1.5,"CENTER");
    if(servo_Center > 0 && servo_Center < 180){
      servo_Center = Run(servo_Center);
      if(servo_Center < 1){
        servo_Center = 1;
      }else if(servo_Center > 179){
        servo_Center = 179;
      }
    }
    servo(0,servo_Center); //END CENTER
    digitalWrite(ledPin, HIGH);
  }else if(ps2x.Button(PSB_R2)){ //GO HOME
    Set_home();
    digitalWrite(ledPin, HIGH);
    delay(100);
  }else if(ps2x.Button(PSB_SELECT)){ //Save Position
     if(countPosition < 20){
       SetPosition(grab,servo_Left,servo_Right,servo_Center); //SetPosition(int grab,int left,int right,int center)
     }
     digitalWrite(ledPin, HIGH);
     delay(100);
     
  }else if(ps2x.Button(PSB_PAD_LEFT)){
    ResetPosition();
    //glcd(1,1,"ResetPosition");
    digitalWrite(ledPin, HIGH);
  }else{
    digitalWrite(ledPin, LOW);
  }
  //glcd(5,1,"%d",grab);
  //glcd(6,1,"%d",servo_Left);
  //glcd(7,1,"%d",servo_Right);
  //glcd(8,1,"%d",servo_Center);
  //glcd(1,1,"%d",countPosition);
  //glcdClear();
  
  delay(100);
}


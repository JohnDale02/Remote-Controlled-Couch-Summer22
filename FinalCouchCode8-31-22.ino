#include <DigiPotX9Cxxx.h>
#include <ps4.h>
#include <PS4Controller.h>
#include <ps4_int.h>
#include <Arduino.h>



int Lights; //Light variable
int Lreset;
int Reverse; //Reverse Variable
int Rreset; 

int REV = 19;   // A2   (white)
int BRAKE = 21;   // A1  (yellow)
int LIGHT = 22;  // A7  (blue)
int HORN =23  ;  // A8   (green)

int Pow = 0;  // max resistance
int powVar = 0;
int steerVar = 0;

int G, R, cG, cR;  // for battery lights
// 12,(L PWM) 13 (R PWM), 14, 15 (For Steering Motor controller)
int EN = 32; //(GREEN)
int R_POW = 25; //(GREY)
int L_POW = 33; //(Yellow)

void onConnect(){
  Serial.println("Connected!.");
  
  Serial.println("\n");
}
DigiPot pot(13,27,26); // digipot initialization (INC, U/D, CSm in order)

void setup() {  // ------------- SETUPT -------------------------------
  Serial.begin(115200);
  PS4.begin();
  Serial.println("Ready.");
  Serial.print("\n");
  PS4.attachOnConnect(onConnect); 
  pot.set(0);

  // --------- LIght and Horn ---------------------------
 
  int Lights = 0; // Setting light variable
  
  pinMode(HORN, OUTPUT);  // HORN pin
  pinMode(LIGHT, OUTPUT); //  LIGHT pin
  
  digitalWrite(HORN, LOW); //  A8 channel / HORN 
  digitalWrite(LIGHT,LOW);   // A7 channel / LIGHTS

  int cG = 0;  // setting lights to zero
  int cR = 0;
 
  // ------------ Steering -----------------------------
  
  pinMode(EN, OUTPUT); // Setting pins to output
  pinMode(L_POW, OUTPUT);
  pinMode(R_POW, OUTPUT);
 
  digitalWrite(L_POW, LOW);
  digitalWrite(R_POW, LOW);
  digitalWrite(EN, LOW); 
  //-------------- Brake and Rev-----------------------------

  int Reverse = 0; // Setting reverse variable
  
  pinMode(BRAKE,OUTPUT);
  pinMode(REV,OUTPUT);

  digitalWrite(BRAKE, LOW); //  A1 channel / Brake 
  digitalWrite(REV,LOW);   // A2 channel / Rev
}
  
void loop() {
  if (PS4.isConnected()) {
  
 // ---------------------------------- HORN -------------------------------
    if (PS4.Triangle()){
      Pow = 0;
       pot.set(Pow);
      Serial.print("HORN (TRIANGLE)");
      Serial.print("\n");
      digitalWrite(HORN, HIGH);  // HORN PIN
      delay(10);
      }
      else{
      digitalWrite(HORN,LOW);
      delay(10);
      }

 // --------------------------- LIGHTS -------------------------------------
    if (PS4.Square()){
      Pow = 0;
      pot.set(Pow);
      if (Lights == 1){
        Lights = 0;
        Lreset = 1;
        delay(200);
        }
      else{
        Lights = 1;
        Lreset = 1 ;
        delay(200); 
        }
    }    
    if (Lights == 1 && Lreset == 1){
      Serial.print("Lights ON");
      Serial.print("\n");
      digitalWrite(LIGHT, HIGH);
      delay(10);
      Lreset = 0;
      }
    
   if (Lights == 0 && Lreset == 1){
    Serial.print(" Lights OFF");
    Serial.print("\n");
    digitalWrite(LIGHT, LOW);
    delay(10);
     Lreset = 0;
      }
 // ------------------------------ STEERING --------------------------
 if (PS4.LStickX() < -40){
  steerVar = 1;
  digitalWrite(EN,HIGH);
    digitalWrite(R_POW, LOW);
    digitalWrite(L_POW, HIGH);
    delay(10);
    
    Serial.print("\n");
   Serial.print("Left");
    }
    
  else if (PS4.LStickX() > 40){
    steerVar = 1;
    digitalWrite(EN,HIGH);
    digitalWrite(L_POW, LOW);
    digitalWrite(R_POW, HIGH);
    delay(10);
    Serial.print("\n");
    Serial.print("Right");
      Serial.print("\n");
     
    }
  else if (PS4.LStickX()<=40 && PS4.LStickX()>=-40 && steerVar == 1){
    steerVar = 0;
    digitalWrite(EN,LOW);
    digitalWrite(R_POW, LOW); 
    digitalWrite(L_POW, LOW);
    delay(10);   
    }
    else{
      }
 

//-----------------------------POWER ---------------------------------

 if(PS4.R2Value() > 5 && PS4.L2Value() == 0 ){
   powVar = 1;
   int PowRem = PS4.R2Value();
   float Pow = map(PowRem, 5, 255, 0, 28);
   delay(10);
   Serial.print(PS4.R2Value());
   Serial.print("\t");
   Serial.print(Pow);
   Serial.print("\n");
 
  pot.set(Pow);
 }
  
  else if (powVar == 1 && PS4.R2Value() <=5 && PS4.L2Value() ==0) {
    powVar =0;
    Pow = 0;
    pot.set(Pow);
    delay(10);
    Serial.print("Not");
    Serial.print("\n");
    
    }
   
   else{
    Serial.print("Nothing");
    Serial.print("\n");
    }

 //------------------- Brake------------------------------------

 if(PS4.L2Value() > 15){
  Pow = 0;
  pot.set(Pow);
   digitalWrite(BRAKE, HIGH);
   delay(10);
   Serial.print("BRAKING");
   Serial.print("\n");
  }
  else{
    digitalWrite(BRAKE,LOW);
    delay(10);
    }

 //------------------ REVERSE-----------------------------

 if (PS4.Options() && PS4.L2Value() > 15){
  Pow = 0;
  pot.set(Pow);
      if (Reverse == 1){
        Reverse = 0;
        Rreset = 1;
        delay(200);
        }
      else{
        Reverse = 1;
        Rreset = 1 ;
        delay(200); 
        }
    }    
    if (Reverse == 1 && Rreset == 1){
      Serial.print("Reverse ON");
      Serial.print("\n");
      digitalWrite(REV, HIGH);
      delay(10);
      Rreset = 0;
      }
    
   if (Reverse == 0 && Rreset == 1){
    Serial.print(" Reverse OFF");
    Serial.print("\n");
    digitalWrite(REV, LOW);
    delay(10);
     Rreset = 0;
      }

  //----------------------------- Battery Life ---------------------
  
  if(PS4.Battery() == 10){
  G=255;
  R=0;
  }
  if(PS4.Battery() == 9){
  G=255;
  R=50;
  }
  if(PS4.Battery() == 8){
  G=255;
  R=95;
  }
  if(PS4.Battery() == 7){
  G=255;
  R=120;
  }
  if(PS4.Battery() == 6){
  G=255;
  R=160;
  }
  if(PS4.Battery() == 5){
  G=255;
  R=200;
  }
  if(PS4.Battery() == 4){
  G=255;
  R=255;
  }
  if(PS4.Battery() == 3){
  G=200;
  R=255;
  }
  if(PS4.Battery() == 2){
  G=165;
  R=255;
  }
  if(PS4.Battery() == 1){
  G=100;
  R=255;
  }
  if(PS4.Battery() == 0){
  G=0;
  R=255;
  }
  
  if (cG != G || cR != R){
    cG = G;
    cR = R;
    PS4.setLed(cR,cG,0);
    PS4.sendToController();
    Serial.print("NEW BATT LEVEL");
    Serial.print("\t");
    Serial.print(PS4.Battery()); // Print Battery 
    } 
      
  }  // ------------------ ON PS4 DISCONNECT ------------------------------
  else{
    digitalWrite(EN,LOW);
    digitalWrite(L_POW, LOW);
    digitalWrite(R_POW, LOW);
    int Pow = 0;  //setting to max resistance
    digitalWrite(LIGHT,LOW);  // off lights
    digitalWrite(HORN, LOW); // off horn
    digitalWrite(L_POW, LOW);  // off turn
    digitalWrite(R_POW,LOW);  // off turn
    digitalWrite(BRAKE, LOW); // turn off brakes
    digitalWrite(REV, LOW);  // turn off reverse
    pot.reset();
    delay(50);
    }
  
    
  }
  
 

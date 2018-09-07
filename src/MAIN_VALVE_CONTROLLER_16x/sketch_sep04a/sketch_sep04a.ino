//#define AVR_MEGA2560
//#define AVR_UNO



volatile bool mode = 1; //AUTO = 0 

#define  MAN_MODE_BTN A0
#define  AUTO_MODE_BTN A1


#ifdef __AVR_ATmega328P__
volatile bool invert_io = true;
#define OUT_COUNT 12

#define UIC_R_INDEX 0
#define UIC_G_INDEX 1
#define UIC_P_INDEX 2
byte pins[OUT_COUNT][3]={
 {1,2,0},
 {2,3,0},
 {3,4,0},
 {4,5,0},
 {5,6,0},
 {6,7,0},
 {7,8,0},
 {8,9,0},
 {9,10,0},
 {10,11,0},
 {11,12,0},
 {12,13,0}
};
#endif


#ifdef __AVR_ATmega2560__


#define UIC_R_INDEX 12
#define UIC_G_INDEX 13
#define UIC_P_INDEX 14

volatile bool invert_io = true;
#define OUT_COUNT 16
byte pins[OUT_COUNT][3]={
 {1,22,0},
 {2,24,0},
 {3,26,0},
 {4,28,0},
 {5,30,0},
 {6,32,0},
 {7,34,0},
 {8,36,0},
 {9,38,0},
 {10,40,0},
 {11,42,0},
 {12,44,0},
 {13,48,0},
 {14,50,0},
 {15,52,0}
};
#endif

String readString;

void setup() {

   Serial.begin(9600);
Serial.println("__START__");
   
  // put your setup code here, to run once:
  for(int i = 0; i < OUT_COUNT; i++){  
    pinMode(pins[i][1], OUTPUT);

    if(!invert_io){
    digitalWrite(pins[i][1], HIGH);
    }else{
    digitalWrite(pins[i][1], LOW);
    }

    delay(500);
     if(!invert_io){
    digitalWrite(pins[i][1], LOW);
    }else{
    digitalWrite(pins[i][1], HIGH);
    }
    
  }


  pinMode(MAN_MODE_BTN, INPUT_PULLUP);
  pinMode(AUTO_MODE_BTN, INPUT_PULLUP);
  
}


String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {
        0, -1};
    int maxIndex = data.length() - 1;
    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void loop() {
  // put your main code here, to run repeatedly:



if(digitalRead(AUTO_MODE_BTN) == LOW){
  delay(100);
  mode = 0;
  Serial.println("__AUTO_MODE__");
  while(digitalRead(AUTO_MODE_BTN) == LOW){
    delay(100);
    }
  }

if(digitalRead(MAN_MODE_BTN) == LOW){
  delay(100);
  mode = 1;
    Serial.println("__MANUAL_MODE__");
  while(digitalRead(MAN_MODE_BTN) == LOW){
    delay(100);
    }
  }
/*
if(mode == 0){
int randNumber = random(OUT_COUNT);
int randNumber_s = random(1);
    if(randNumber_s){
    digitalWrite(pins[randNumber][1], LOW);
    pins[randNumber][2] = 0;
    }else{
    digitalWrite(pins[randNumber][1], HIGH);
     pins[randNumber][2] = 1;
    }

  delay(2000);
return;
}

 */
while (Serial.available())
    {
        delay(5); //delay to allow buffer to fill
        if (Serial.available() > 0)
        {
            char c = Serial.read(); //gets one byte from serial buffer
            if(c == '\r' || c == '\n'){
              break;
              }
            readString += c;        //makes the string readString
            Serial.println(c);
        }
    }

    if (readString.length() > 0)
    {
        String cmd = getValue(readString, '_', 0);
        int pin = getValue(readString, '_', 1).toInt();
        int state = getValue(readString, '_', 2).toInt();
        
        Serial.println("GOT_" + cmd + "_" + String(pin) + "->" + String(state));
        readString = "";

if(cmd == "SETUIC" || cmd == "setuic"){

if(!invert_io){
    digitalWrite(pins[UIC_R_INDEX][1], LOW);
     digitalWrite(pins[UIC_G_INDEX][1], LOW);
      digitalWrite(pins[UIC_P_INDEX][1], LOW);
      pins[UIC_P_INDEX][2] = 0;
    }else{
    digitalWrite(pins[UIC_R_INDEX][1], HIGH);
    digitalWrite(pins[UIC_G_INDEX][1], HIGH);
    digitalWrite(pins[UIC_P_INDEX][1], HIGH);
    pins[UIC_P_INDEX][2] = 1;
   }

if(state == 0){if(!invert_io){digitalWrite(pins[UIC_R_INDEX][1], HIGH);pins[UIC_R_INDEX][2] = 1;}else{digitalWrite(pins[UIC_R_INDEX][1], LOW);pins[UIC_R_INDEX][2] = 0;}}
if(state == 1){if(!invert_io){digitalWrite(pins[UIC_G_INDEX][1], HIGH);pins[UIC_G_INDEX][2] = 1;}else{digitalWrite(pins[UIC_G_INDEX][1], LOW);pins[UIC_G_INDEX][2] = 0;}}
if(state == 2){if(!invert_io){digitalWrite(pins[UIC_P_INDEX][1], HIGH);pins[UIC_P_INDEX][2] = 1;}else{digitalWrite(pins[UIC_P_INDEX][1], LOW);pins[UIC_P_INDEX][2] = 0;}}
  }
  
if(cmd == "RESET" || cmd == "reset"){
  for(int i = 0; i < OUT_COUNT; i++){  
    if(!invert_io){
    digitalWrite(pins[i][1], LOW);
    pins[i][2] = 0;
    }else{
    digitalWrite(pins[i][1], HIGH);
     pins[i][2] = 1;
    }
    
  }
  }

if(cmd == "SET" || cmd == "set"){
  for(int i = 0; i < OUT_COUNT; i++){  
  if(pins[i][0] != pin){continue;}

   if(state ==1){
    if(!invert_io){
    digitalWrite(pins[i][1], LOW);
     pins[i][2] = 0;
    }else{
    digitalWrite(pins[i][1], HIGH);
     pins[i][2] = 1;
    }
   }else{
    if(!invert_io){
    digitalWrite(pins[i][1], HIGH);
     pins[i][2] = 1;
    }else{
    digitalWrite(pins[i][1], LOW);
     pins[i][2] = 0;
    }
    }
  }
}


        
    }
}

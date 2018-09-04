#define ARD_UNO
//#define ARD_MEGA


volatile bool invert_io = true;


#define UIC_R_INDEX 0
#define UIC_G_INDEX 1
#define UIC_P_INDEX 2

#ifdef ARD_UNO
#define OUT_COUNT 12
byte pins[OUT_COUNT][2]={
 {1,2},
 {2,3},
 {3,4},
 {4,5},
 {5,6},
 {6,7},
 {7,8},
 {8,9},
 {9,10},
 {10,11},
 {11,12},
 {12,13}
};
#endif


#ifdef ARD_MEGA
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
 {12,48,0},
 {12,50,0},
 {12,52,0}
};
#endif

String readString;

void setup() {

   Serial.begin(9600);

   
  // put your setup code here, to run once:
  for(int i = 0; i < OUT_COUNT; i++){  
    pinMode(pins[i][1], OUTPUT);

    if(!invert_io){
    digitalWrite(pins[i][1], LOW);
    }else{
    digitalWrite(pins[i][1], HIGH);
    }
    
  }
  
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
while (Serial.available())
    {
        delay(3); //delay to allow buffer to fill
        if (Serial.available() > 0)
        {
            char c = Serial.read(); //gets one byte from serial buffer
            readString += c;        //makes the string readString
        }
    }

    if (readString.length() > 0)
    {
        String cmd = getValue(readString, '_', 0);
        int pin = getValue(readString, '_', 1).toInt();
        int state = getValue(readString, '_', 2).toInt();
        
        Serial.println("GOT_" + cmd + "_" + String(pin) + "->" + String(state));
        readString = "";

if(cmd == "SETUIC"){

if(!invert_io){
    digitalWrite(pins[UIC_R_INDEX][1], LOW);
     digitalWrite(pins[UIC_G_INDEX][1], LOW);
      digitalWrite(pins[UIC_P_INDEX][1], LOW);
    }else{
    digitalWrite(pins[UIC_R_INDEX][1], HIGH);
    digitalWrite(pins[UIC_G_INDEX][1], HIGH);
    digitalWrite(pins[UIC_P_INDEX][1], HIGH);
   }

if(state == 0){if(!invert_io){digitalWrite(pins[UIC_R_INDEX][1], HIGH);}else{digitalWrite(pins[UIC_R_INDEX][1], LOW);}}
if(state == 1){if(!invert_io){digitalWrite(pins[UIC_G_INDEX][1], HIGH);}else{digitalWrite(pins[UIC_G_INDEX][1], LOW);}}
if(state == 2){if(!invert_io){digitalWrite(pins[UIC_P_INDEX][1], HIGH);}else{digitalWrite(pins[UIC_P_INDEX][1], LOW);}}
  }
  
if(cmd == "RESET"){
  for(int i = 0; i < OUT_COUNT; i++){  
    if(!invert_io){
    digitalWrite(pins[i][1], LOW);
    }else{
    digitalWrite(pins[i][1], HIGH);
    }
    
  }
  }

if(cmd == "SET"){
  for(int i = 0; i < OUT_COUNT; i++){  
  if(pins[i][0] != pin){continue;}

   if(state ==1){
    if(!invert_io){
    digitalWrite(pins[i][1], LOW);
    }else{
    digitalWrite(pins[i][1], HIGH);
    }
   }else{
    if(!invert_io){
    digitalWrite(pins[i][1], HIGH);
    }else{
    digitalWrite(pins[i][1], LOW);
    }
    }
  }
}


        
    }
}

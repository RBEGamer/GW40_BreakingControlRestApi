#include <Arduino.h>

#define EN 5
#define STEP 6
#define DIR 7

#define ENCODER_A 2
#define ENCODER_B 3

#define RESET_BTN 4
#define SW_OBEN 8
#define SW_UNTEN 9

#define EXT_TR_P A0
#define EXT_TR_R A1
#define EXT_TR_G A2

#define ERR_LED 12
#define ERR_LED_G 11
char stellung_curr = 'x';
char stellung_dest = 'x';
String readString;



volatile boolean fired;
volatile boolean up;




// Interrupt Service Routine
void isr ()
{
 if (digitalRead (ENCODER_A)){
   up = digitalRead (ENCODER_B);
   
 }else{
   up = !digitalRead (ENCODER_B);
 }                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
 fired = true;
}  // end of isr







void setup()
{

  digitalWrite (ENCODER_A, HIGH);   // activate pull-up resistors
 digitalWrite (ENCODER_A, HIGH); 
 
 attachInterrupt (0, isr, CHANGE);   // pin 2
 attachInterrupt (1, isr, CHANGE);   // pin 3


 pinMode(EXT_TR_P, INPUT);
  pinMode(EXT_TR_R, INPUT);
   pinMode(EXT_TR_G, INPUT);
   
    // put your setup code here, to run once:
    pinMode(EN, OUTPUT);   //Enable
    pinMode(STEP, OUTPUT); //Step
    pinMode(DIR, OUTPUT);  //Direction

    pinMode(SW_OBEN, INPUT_PULLUP);
    pinMode(SW_UNTEN, INPUT_PULLUP);

    pinMode(RESET_BTN, INPUT_PULLUP);
    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);

    digitalWrite(DIR, LOW);
    digitalWrite(EN, HIGH);


pinMode(ERR_LED, OUTPUT);
digitalWrite(ERR_LED, LOW);

pinMode(ERR_LED_G,OUTPUT);
digitalWrite(ERR_LED_G, LOW);
    Serial.begin(9600);


 stellung_dest = 'r';
    move_to_dest();
    
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

void get_pos()
{
  delay(1);
    bool swo = !digitalRead(SW_OBEN);
    bool swu = !digitalRead(SW_UNTEN);

    if (!swo && swu)
    {
        stellung_curr = 'r';
    }
    else if (swo && !swu)
    {
        stellung_curr = 'p';
    }
    else if (swo && swu)
    {
        stellung_curr = 'g';
    }else{
      stellung_curr = 'x';
      }



if(stellung_curr == 'x'){
  digitalWrite(ERR_LED,LOW);
  digitalWrite(ERR_LED_G, LOW);
  }else if(stellung_curr == 'r'){
   digitalWrite(ERR_LED,LOW);
digitalWrite(ERR_LED_G, HIGH);
}else if(stellung_curr == 'p'){
   digitalWrite(ERR_LED,HIGH);
digitalWrite(ERR_LED_G, LOW);
}else if(stellung_curr == 'g'){
   digitalWrite(ERR_LED,HIGH);
digitalWrite(ERR_LED_G, HIGH);
   }
    
}





void move_to_dest(){
  int err_add = 0;
    if (stellung_curr != stellung_dest && stellung_dest != 'x')
    {

        bool dirok = true;
        if (stellung_curr == 'r' && stellung_dest == 'p')
        {
            digitalWrite(DIR, LOW);
        }
        else if (stellung_curr == 'r' && stellung_dest == 'g')
        {
            digitalWrite(DIR, LOW);
        }
        else if (stellung_curr == 'p' && stellung_dest == 'g')
        {
            digitalWrite(DIR, LOW);
        }
        else if (stellung_curr == 'p' && stellung_dest == 'r')
        {
            digitalWrite(DIR, HIGH);
        }
        else if (stellung_curr == 'g' && stellung_dest == 'p')
        {
            digitalWrite(DIR, HIGH);
        }
        else if (stellung_curr == 'g' && stellung_dest == 'r')
        {
            digitalWrite(DIR, HIGH);
        }
        else
        {
            Serial.println("ERR DIR STELLUNG");
            digitalWrite(ERR_LED_G, LOW);
             delay(100);
            digitalWrite(ERR_LED_G, HIGH);
            delay(100);
            digitalWrite(ERR_LED_G, LOW);
            delay(100);
            digitalWrite(ERR_LED, HIGH);
            delay(100);
            digitalWrite(ERR_LED, LOW);
            delay(100);
            digitalWrite(ERR_LED, HIGH);
            delay(100);
            digitalWrite(ERR_LED, LOW);
            dirok = false;
            err_add = 200;
        }

        digitalWrite(EN, LOW);
        for (int Index = 0; Index < 200+err_add; Index++)
        {
            digitalWrite(STEP, HIGH);
            delayMicroseconds(1000);
            digitalWrite(STEP, LOW);
            delayMicroseconds(1000);

            get_pos();
            if (stellung_curr == stellung_dest)
            {

                if (stellung_curr != 'r')
                {
                    for (int Index1 = 0; Index1 < 15; Index1++)
                    {
                        digitalWrite(STEP, HIGH);
                        delayMicroseconds(1000);
                        digitalWrite(STEP, LOW);
                        delayMicroseconds(1000);
                    }
                }

                break;
            }
            delay(1);
        }
        digitalWrite(EN, HIGH);
}
}

void loop()
{
    // put your main code here, to run repeatedly:


if(digitalRead(EXT_TR_R) == LOW){
  stellung_dest == 'r';
  Serial.println("EXT_TR R");
  move_to_dest();
}else if(digitalRead(EXT_TR_G) == LOW){
  stellung_dest == 'g';
  Serial.println("EXT_TR G");
  move_to_dest();
  }else if(digitalRead(EXT_TR_P) == LOW){
    stellung_dest == 'p';
    Serial.println("EXT_TR P");
    move_to_dest();
  }else{
    Serial.println("EXT_TR FAILED");
    }
    
if (fired){
   if (up){
 digitalWrite(DIR, HIGH);
    digitalWrite(EN, LOW);
    for (int Index1 = 0; Index1 < 5; Index1++)
                    {
                        digitalWrite(STEP, HIGH);
                        delayMicroseconds(1000);
                        digitalWrite(STEP, LOW);
                        delayMicroseconds(1000);
                    }
                    digitalWrite(EN, HIGH);
    
    }else{
      /*
      digitalWrite(DIR, LOW);
    digitalWrite(EN, LOW);
    for (int Index1 = 0; Index1 < 5; Index1++)
                    {
                        digitalWrite(STEP, HIGH);
                        delayMicroseconds(1000);
                        digitalWrite(STEP, LOW);
                        delayMicroseconds(1000);
                    }
                    digitalWrite(EN, HIGH);
                    */
      }
     
   fired = false;
       

  
   }  // end if fired



    get_pos();

    Serial.println(stellung_curr);

    if (digitalRead(RESET_BTN) == LOW)
    {
        if (stellung_dest == 'r')
        {
          Serial.println("r->g by user");
            stellung_dest = 'g';
        }
        else if (stellung_curr == 'g')
        {
           Serial.println("g->p by user");
            stellung_dest = 'p';
        }
        else
        {
          Serial.println("->r by user");
            stellung_dest = 'r';
        }
        while (!digitalRead(RESET_BTN))
        {
            delay(10);
        }
        Serial.println("move_to_dest");
        move_to_dest();
    }

    

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
        stellung_dest = 'x';
        if (getValue(readString, '_', 0) == "p")
        {
            stellung_dest = 'p';
        }
        else if (getValue(readString, '_', 0) == "g")
        {
            stellung_dest = 'g';
        }
        else if (getValue(readString, '_', 0) == "r")
        {
            stellung_dest = 'r';
        }

       

        move_to_dest();
        readString = "";
    }
}

 

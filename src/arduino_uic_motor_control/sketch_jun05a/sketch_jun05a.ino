
#define EN 4
#define STEP 2
#define DIR 3


#define SW_OBEN 8
#define SW_UNTEN 9

char stellung_curr = 'x';
char stellung_dest = 'x';
String readString;
void setup() {
  // put your setup code here, to run once:
 pinMode(EN, OUTPUT); //Enable
  pinMode(STEP, OUTPUT); //Step
  pinMode(DIR, OUTPUT); //Direction

pinMode(SW_OBEN, INPUT_PULLUP);
pinMode(SW_UNTEN, INPUT_PULLUP);



 digitalWrite(DIR,LOW);
   digitalWrite(EN,HIGH);

  Serial.begin(9600);
}





String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
    0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
  }

void get_pos(){
  bool swo = !digitalRead(SW_OBEN);
bool swu = !digitalRead(SW_UNTEN);


if(!swo && swu){
  stellung_curr = 'r';
  }else if(swo && ! swu){
    stellung_curr = 'p';
   }else if(swo && swu){
    stellung_curr = 'g';
    }
  }


  
void loop() {
  // put your main code here, to run repeatedly:


get_pos();
    
Serial.println(stellung_curr);

while (Serial.available()) {
      delay(3);  //delay to allow buffer to fill
      if (Serial.available() >0) {
        char c = Serial.read();  //gets one byte from serial buffer
        readString += c; //makes the string readString
      }
    }

    if (readString.length() >0) {
stellung_dest = 'x';
      if(getValue(readString, '_', 0) == "p"){
        stellung_dest = 'p';
      }else if(getValue(readString, '_', 0) == "g"){
        stellung_dest = 'g';
      }else if(getValue(readString, '_', 0) == "r"){
        stellung_dest = 'r';
      }
    
      




if(stellung_curr != stellung_dest && stellung_dest != 'x'){


bool dirok = true;
if(stellung_curr == 'r' && stellung_dest == 'p'){
  digitalWrite(DIR,LOW);
  }else if(stellung_curr == 'r' && stellung_dest == 'g'){
  digitalWrite(DIR,LOW);
  }else if(stellung_curr == 'p' && stellung_dest == 'g'){
  digitalWrite(DIR,LOW);
  }else if(stellung_curr == 'p' && stellung_dest == 'r'){
  digitalWrite(DIR,HIGH);
  }else if(stellung_curr == 'g' && stellung_dest == 'p'){
  digitalWrite(DIR,HIGH);
  }else if(stellung_curr == 'g' && stellung_dest == 'r'){
  digitalWrite(DIR,HIGH);
  }else{
    Serial.println("ERR DIR STELLUNG");
    dirok = false;
    }



digitalWrite(EN,LOW);
  for(int Index = 0; Index < 200; Index++)
  {
    digitalWrite(STEP,HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP,LOW);
    delayMicroseconds(1000);

      get_pos();
    if(stellung_curr == stellung_dest){

if(stellung_curr != 'r'){
      for(int Index1 = 0; Index1 < 15; Index1++)
  {
    digitalWrite(STEP,HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP,LOW);
    delayMicroseconds(1000);

  }
}

      break;
      }
      delay(1);
  }
  digitalWrite(EN,HIGH);

  

  
  }
      readString = "";
    }


 


  
  
}


/*
 * 
 * 
 * 
 * 
 *         
        digitalWrite(DIR,LOW);
        digitalWrite(EN,LOW);
        for(int Index = 0; Index < 200; Index++)
  {
    digitalWrite(STEP,HIGH);
    delayMicroseconds(7000);
    digitalWrite(STEP,LOW);
    delayMicroseconds(700);

    if(swo && swu){
      break;
      }
      delay(1);
  }
  digitalWrite(EN,HIGH);
  */
 

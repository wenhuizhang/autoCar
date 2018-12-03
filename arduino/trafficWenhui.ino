/*
Traffic Light
*/

const int r = 9;             //connect red led at pin 9    
const int y = 10;           //connect yellow led at pin 10
const int g = 11;           //connect green led at pin 11
const int sec = 1000;       //seconds defined 
void setup()
  {
    pinMode(r,OUTPUT);
    pinMode(y,OUTPUT);
    pinMode(g,OUTPUT);
    delay(sec);
  }

void loop()
    {   
        digitalWrite(r,LOW) ;
        delay(sec*5);
        digitalWrite(r,HIGH) ;
        digitalWrite(y,LOW) ;
        delay(sec*5);
        digitalWrite(y,HIGH) ;
        digitalWrite(g,LOW) ;
        delay(sec*5);
        digitalWrite(g,HIGH) ;

    }

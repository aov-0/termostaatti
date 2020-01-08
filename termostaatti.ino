//Määrittää pinneille kuvaavat nimet, esim. ANTURI lämpöanturille
#define ANTURI A0
#define POTENTIO A1
#define FAN 11
#define LEDPUN 12
#define LEDVIH 13

int LUKEMA;
int ASETUS;

void setup(){
  pinMode(LEDPUN, OUTPUT);
  pinMode(LEDVIH, OUTPUT);
  pinMode(FAN, OUTPUT);
  analogWrite(FAN, 0);
  Serial.begin(9600);
}

void loop() {
  LUKEMA = analogRead(ANTURI);
  float ASTE = (-9.00/70.00)*LUKEMA+91;
  Serial.print(ASTE);
  Serial.println(" astetta");
  ASETUS = analogRead(POTENTIO);
  Serial.println(ASETUS);
  delay(500);
  if(ASTE > 23){
    analogWrite(FAN, 255);
  }else if(ASTE < 23){
    analogWrite(FAN, 0);
  }
 }

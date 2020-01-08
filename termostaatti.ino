//Määrittää pinneille kuvaavat nimet, esim. ANTURI lämpöanturille
#define ANTURI A0
#define POTENTIO A1
#define FAN 11
#define LEDPUN 12
#define LEDVIH 13

//Selvyyden vuoksi ledin sytyttämiselle/sammuttamiselle omat nimitykset
#define LEDON HIGH
#define LEDOFF LOW

int LUKEMA;
int ASETUS;
int PWM;

void setup(){
  pinMode(LEDPUN, OUTPUT);
  pinMode(LEDVIH, OUTPUT);
  pinMode(FAN, OUTPUT);
  analogWrite(FAN, 0);    //Varmistetaan että tuuletin ei lähde käyntiin ohjelman alkaessa
  Serial.begin(9600);
}

void loop() {
  LUKEMA = analogRead(ANTURI);
  float ASTE = (-9.00/70.00)*LUKEMA+91;
  Serial.print("Mitattu lämpötila: ");
  Serial.print(ASTE);
  Serial.println(" astetta");
  ASETUS = analogRead(POTENTIO);
  ASETUS = map(ASETUS, 0, 1023, 16, 30);
  Serial.print("Raja-arvo: ");
  Serial.print(ASETUS);
  Serial.println(" astetta");
  delay(1000);
  digitalWrite(LEDVIH, LEDON);
  //PWM = map(FAN, 60, 255, 1, 7);
  if(ASTE > ASETUS){
    digitalWrite(LEDVIH, LEDOFF);
    digitalWrite(LEDPUN, LEDON);
    analogWrite(FAN, 180);
  }else if(ASTE < ASETUS + 2){
    digitalWrite(LEDPUN, LEDOFF);
    analogWrite(FAN, 0);
  }
 }

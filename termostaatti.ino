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

//Halutut toiminnot löytyy ohjelmasta mutta vaatii säätämistä
//

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
  ASETUS = map(ASETUS, 0, 1023, 160, 300);    //160 & 300 potentiometrin halutut ääriasennot (alunperin minimi 16 astetta, max 30 astetta), lisättiin nollat perään jotta saatiin "askellusta" lisää.
  Serial.print("Raja-arvo: ");
  Serial.print(ASETUS);
  Serial.println(" astetta");
  delay(1000);
  digitalWrite(LEDVIH, LEDON);
  ASTE*=10;
  PWM = map(ASTE - ASETUS, 10, 70, 60, 255);
  Serial.println(PWM);
  if(PWM>0){
    analogWrite(FAN, PWM);
  }else{
    analogWrite(FAN, 0);
  }
  if(ASTE > ASETUS){
    digitalWrite(LEDVIH, LEDOFF);
    digitalWrite(LEDPUN, LEDON);
    //analogWrite(FAN, PWM);
  }else if(ASTE < ASETUS - 3){
    digitalWrite(LEDPUN, LEDOFF);
    //analogWrite(FAN, 0);
  }
 }

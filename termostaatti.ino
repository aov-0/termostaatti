//Määrittää pinneille kuvaavat nimet, esim. ANTURI lämpöanturille
#define ANTURI A0
#define POTENTIO A1
#define FAN 11
#define LEDPUNAINEN 12
#define LEDVIHREA 13

int LUKEMA;
int ASETUS;

void setup(){
  pinMode(LEDPUNAINEN, OUTPUT);
  pinMode(LEDVIHREA, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  LUKEMA = analogRead(ANTURI);
  Serial.println(LUKEMA);
  ASETUS = analogRead(POTENTIO);
  Serial.println(ASETUS);
  delay(500);
 }

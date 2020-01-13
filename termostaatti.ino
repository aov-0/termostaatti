//Vakiot, määrittää pinneille kuvaavat nimet, esim. ANTURI lämpöanturille
#define ANTURI A0
#define POTENTIO A1
#define FAN 11
#define LEDPUN 12
#define LEDVIH 13

//Selkeyden vuoksi ledin sytyttämiselle/sammuttamiselle omat nimitykset
#define LEDON HIGH
#define LEDOFF LOW

//Globaalit muuttujat
int LUKEMA;
int ASETUS;
int OHJAUS;

//Halutut toiminnot löytyy ohjelmasta mutta vaatii säätämistä

//Funktiot
void setup(){
  //Funktiokutsut
  pinMode(LEDPUN, OUTPUT);      
  pinMode(LEDVIH, OUTPUT);
  pinMode(FAN, OUTPUT);
  analogWrite(FAN, 0);    //Varmistetaan että tuuletin on "nolla" tilassa käynnistettäessä, eli ei lähde pyörimään
  Serial.begin(9600);     
}

void loop() {
  LUKEMA = analogRead(ANTURI);              //Luetaan lämpötila anturista
  float ASTE = (-9.00/70.00)*LUKEMA+91;     //float on vastaava kuin int, mutta se antaa käyttää desimaalilukuja
  Serial.print("Mitattu lämpötila: ");      //Sarjaporttiin tulostus seuraavat 3 riviä
  Serial.print(ASTE);
  Serial.println(" astetta");
  ASETUS = analogRead(POTENTIO);            //Lukee potentoimetrin "asennon" tai vastusarvon
  ASETUS = map(ASETUS, 0, 1023, 160, 300);    //Muuntaa potentiometrin äärilukemat 160 ja 300 arvoisiksi (0 = 160 ja 1023 = 300)
  Serial.print("Raja-arvo: ");
  Serial.print(ASETUS);
  Serial.println(" astetta");
  delay(1000);                              //Viive jottei sarjaporttiin tulostuisi niin usein
  digitalWrite(LEDVIH, LEDON);              //Vihreä ledi palaa aina kun lämpötila on halutun arvoinen; silloin kun on jäähdytetty tarpeeksi matalalle
  ASTE*=10;                                   //Kertoo luetun lämpötilan 10 kertaisesti
  OHJAUS = map(ASTE - ASETUS, 10, 70, 60, 255);
  Serial.println(OHJAUS);
  if(OHJAUS>0){
    analogWrite(FAN, OHJAUS);
  }else{
    analogWrite(FAN, 0);
  }
  if(ASTE > ASETUS){
    digitalWrite(LEDVIH, LEDOFF);
    digitalWrite(LEDPUN, LEDON);
    //analogWrite(FAN, OHJAUS);
  }else if(ASTE < ASETUS - 3){
    digitalWrite(LEDPUN, LEDOFF);
    //analogWrite(FAN, 0);
  }
 }

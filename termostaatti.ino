//Vakiot, määrittää pinneille kuvaavat nimet, esim. ANTURI lämpöanturille
#define ANTURI A0
#define POTENTIO A1
#define FAN 11
#define LEDSIN 12
#define LEDVIH 13
//Selkeyden vuoksi ledin sytyttämiselle/sammuttamiselle omat määritykset
#define LEDON HIGH
#define LEDOFF LOW

//Globaalit muuttujat
int LUKEMA;
int ASETUS;
int OHJAUS;
int SUMMA;
int TAULU[5];     //Muuttuja keskiarvoistamista varten.
int INDEX = 0;

//Funktiot
void setup(){
  //Funktiokutsut
  pinMode(LEDSIN, OUTPUT);
  pinMode(LEDVIH, OUTPUT);
  pinMode(FAN, OUTPUT);
  analogWrite(FAN, 0);    //Varmistetaan että tuuletin on "nolla" tilassa käynnistettäessä, eli ei lähde pyörimään
  Serial.begin(9600);
  for(int i = 0; i<5; i++){           //for silmukassa "i" muuttujaan tallennetaan i:n arvoksi 0, ja niin kauan kun i:n arvo on alle 5, lisätään lukemaan joka kierrolla +1.
   TAULU[i] =  analogRead(ANTURI);    //Lukee anturin arvon, keskiarvoistaa lukeman ja tallentaa TAULU muuttujaan. Tehdään siksi ettei puhallin lähde heti alkuun pyörimään.
  }
}

void loop() {
  TAULU[INDEX++] = analogRead(ANTURI);              //Luetaan "lämpötila" anturista, ja tehdään mittauksista...
  if(INDEX == 5){
    INDEX = 0;
  }
  int SUMMA=0;
  for(int i = 0; i<5; i++){
    SUMMA = SUMMA+TAULU[i];
  }
  LUKEMA = SUMMA/5;                                  //...keskiarvoistus
  float ASTE = (-9.00/70.00)*LUKEMA+91;     //float on vastaava kuin int, mutta se antaa käyttää desimaalilukuja. Lukemien muuntamisessa on käytetty suoran yhtälöä.
  Serial.print("Mitattu lämpötila: ");      //Sarjaporttiin tulostus seuraavat 3 riviä
  Serial.print(ASTE);
  Serial.println(" astetta");
  ASETUS = analogRead(POTENTIO);            //Lukee potentoimetrin "asennon" tai vastusarvon
  ASETUS = map(ASETUS, 0, 1023, 160, 300);    //Muuntaa potentiometrin äärilukemat 0 ja 1023, 160 ja 300 arvoisiksi (0 = 160 ja 1023 = 300)
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
    digitalWrite(LEDSIN, LEDON);
  }else if(ASTE < ASETUS){
    digitalWrite(LEDSIN, LEDOFF);
  }
 }

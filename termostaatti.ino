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
int TAULU[5];     //Muuttuja keskiarvoistamista varten. Varaa 5 tallennuspaikkaa tähän käyttöön.
int INDEX = 0;    //Alustetaan arvoltaan 0:ksi.

//Funktiot
void setup(){
  //Funktiokutsut
  pinMode(LEDSIN, OUTPUT);
  pinMode(LEDVIH, OUTPUT);
  pinMode(FAN, OUTPUT);
  digitalWrite(LEDVIH, LEDOFF);       //Alustetaan ledit pois-päältä tilaan
  digitalWrite(LEDSIN, LEDOFF);       //-..-
  analogWrite(FAN, 0);                //Varmistetaan että tuuletin on "nolla" tilassa käynnistettäessä, eli ei lähde pyörimään
  Serial.begin(9600);
  for(int i = 0; i<5; i++){           //for silmukassa "i" muuttujaan tallennetaan i:n arvoksi 0, ja niin kauan kunnes i:n arvo on 5, lisätään lukemaan joka kierron jälkeen +1.
   TAULU[i] =  analogRead(ANTURI);    //Lukee anturin arvon, tallentaa TAULU muuttujaan. Tässä tapahtuu keskiarvoistus. Tehdään tässä aluksi siksi ettei puhallin lähde heti alkuun pyörimään.
  }
}

void loop() {
  TAULU[INDEX++] = analogRead(ANTURI);              //Luetaan anturin arvo. INDEX pitää kirjaa mittauksien lukumäärästä.
  if(INDEX == 5){                                 //Jos luettuja arvoja on 5,
    INDEX = 0;                                    //palataan takaisin 0:aan.
  }
  int SUMMA = 0;                     //Alustaa SUMMA:n aloitusarvoksi 0
  for(int i = 0; i<5; i++){        //Niin kauan kunnes on luettu 5 näytettä
    SUMMA = SUMMA+TAULU[i];
  }
  LUKEMA = SUMMA/5;                                  //Laskee keskiarvon edellisessä luetuista lukemista
  float ASTE = (-5.30/34.00)*LUKEMA+105.9;    //float on vastaava kuin int, mutta se antaa käyttää desimaalilukuja. Lukemien muuntamisessa on käytetty suoran yhtälöä.
  Serial.print("Mitattu lämpötila: ");      //Sarjaporttiin tulostus seuraavat 3 riviä
  Serial.print(ASTE);
  Serial.println(" astetta");
  ASETUS = analogRead(POTENTIO);                //Lukee potentiometrin "asennon" tai vastusarvon
  ASETUS = map(ASETUS, 0, 1023, 1600, 3000);    //Muuntaa potentiometrin äärilukemat 0 ja 1023, 1600 ja 3000 arvoisiksi (0 = 1600 ja 1023 = 3000). Näin tehtiin koska haluttiin potentiometrin säädölle enemmän "askellusta", jolloin säätämisen tarkkuus parani. Tässä haetaan 16 asteen minimiarvoa ja 30 asteen maksimiarvoa.
  Serial.print("Raja-arvo: ");
  Serial.print(ASETUS/100.0);               //ASETUS arvo jaetaan sadalla koska se aiemmin mäpättiin sata kertaiseksi lisätarkkuuden saavuttamiseksi. Lisättäessä piste luvun perään, saadaan sarjaporttiin desimaalilukuja.
  Serial.println(" astetta");
  delay(1000);                                    //Viive jottei sarjaporttiin tulostuisi niin usein
  ASTE*=100;                                      //Kertoo luetun lämpötilan 100 kertaisesti, koska haluttiin säätöön enemmän tarkkuutta.
  OHJAUS = map(ASTE - ASETUS, 100, 700, 60, 255); //Jos mitatun arvon ja asetetun arvon erotus on vaikka 1 (tässä yhteydessä 100, koska luvut kerrottiin), mäppäytyy OHJAUS:ksen analogWrite arvo silloin 60, jolloin puhallin pyörii hiljaisimmalla mahdollisella nopeudella. 255 on suurin nopeus.
  Serial.println(OHJAUS);                         //Tulostaa arvon helpottaaksi kehitystyötä
  if(OHJAUS>0){                                   //Jos ohjauksen arvo on enemmän kuin 0,
    analogWrite(FAN, OHJAUS);                     //käynnistetään puhallin "OHJAUS" arvon mukaisella nopeudella
  }else{
    analogWrite(FAN, 0);
  }
  if(ASTE > ASETUS){                //Jos mitattu arvo on enemmän kuin asetettu arvo,
    digitalWrite(LEDVIH, LEDOFF);   //vihreä ledi sammutetaan,
    digitalWrite(LEDSIN, LEDON);    //ja sininen ledi sytytetään.
  }else if(ASTE < ASETUS){          //Jos mitattu arvo on vähemmän kuin asetettu arvo,
    digitalWrite(LEDSIN, LEDOFF);   //sammutetaan sininen ledi
    digitalWrite(LEDVIH, LEDON);    //Vihreä ledi palaa aina kun lämpötila on halutun arvoinen
  }
 }

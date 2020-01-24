//Vakiot, määrittää pinneille kuvaavat nimet, esim. ANTURI lämpöanturille.
#define ANTURI A0
#define POTENTIO A1
#define FAN 11
#define LEDSIN 12
#define LEDVIH 13

//Selkeyden vuoksi ledin sytyttämiselle/sammuttamiselle omat määritykset.
#define LEDON HIGH
#define LEDOFF LOW

//#define debug     //Kommenttina siksi että voi tarpeen mukaan hyödyntää. Jos ei käytössä, hyppää halutun rivin yli koodissa.

//Globaalit muuttujat, jokainen funktio pystyy hyödyntämään niitä.
int LUKEMA;
int ASETUS;
int OHJAUS;
int TAULU[5];     //Muuttuja keskiarvoistamista varten. Varaa 5 tallennuspaikkaa tähän käyttöön.
int INDEX = 0;    //Alustetaan arvoltaan 0:ksi.
float ASTE;       //float on vastaava kuin int, mutta silloin voidaan käyttää desimaalilukuja. 

//Funktiot
void setup(){
  pinMode(LEDSIN, OUTPUT);
  pinMode(LEDVIH, OUTPUT);
  pinMode(FAN, OUTPUT);
  digitalWrite(LEDVIH, LEDOFF);       //Alustetaan ledit pois-päältä tilaan.
  digitalWrite(LEDSIN, LEDOFF);       //-..-
  analogWrite(FAN, 0);                //Varmistetaan että tuuletin on "nolla" tilassa käynnistettäessä, eli ei lähde pyörimään.
  Serial.begin(9600);
  for(int i = 0; i<5; i++){           //for silmukassa "i" muuttujaan tallennetaan i:n arvoksi 0, ja niin kauan kunnes i:n arvo on 5, lisätään lukemaan joka kierron jälkeen +1.
   TAULU[i] =  analogRead(ANTURI);    //Lukee anturin arvon ja tallentaa TAULU muuttujaan. Tämä tehdään void setup:iin siksi ettei puhallin lähde heti alkuun pyörimään.
  }
}

void keskiarvo(){                             //Keskiarvoistukselle oma funtio.
   TAULU[INDEX++] = analogRead(ANTURI);       //Luetaan anturin arvo. INDEX pitää kirjaa mittauksien lukumäärästä.
  if(INDEX == 5){                             //Jos INDEX:iin on luettu 5 arvoa,
    INDEX = 0;                                //palataan laskuissa takaisin 0:aan.
  }
  int SUMMA = 0;                          //Luo muuttujan tätä funktiota varten (sitä ei tarvita muualla ohjelmassa), ja alustaa sen arvoksi 0.
  for(int i = 0; i<5; i++){               //Niin kauan kunnes on luettu 5 näytettä,
    SUMMA = SUMMA+TAULU[i];               //tallentuu aiemmin luettu arvo SUMMA yhteenlaskuun.
  }
  LUKEMA = SUMMA/5;                       //Jakaa luetut 5 arvoa luvulla 5, ja tallettaa sen LUKEMA nimiseksi.
}

void mittaus(){
  ASTE = (-5.30/34.00)*LUKEMA+105.9;                //Lukemien muuntamisessa on käytetty suoran yhtälöä.
  //ASTE = map(LUKEMA, 510.0, 544.0, 26.4 , 21.1);  //Lukemat voidaan muuntaa myös näin mäppäämällä.
  Serial.print("Mitattu lämpötila: ");              //Sarjaporttiin tulostus.
  Serial.print(ASTE);                               //Tulostaa sarjaporttiin suoran yhtälön tuloksen, eli loogisemmat celsius aste lukemat.
  Serial.println(" astetta");                       //Sarjaporttiin tulostus rivinvaihdolla.
  
}

void asetettu(){
  ASETUS = analogRead(POTENTIO);                  //Lukee potentiometrin AD arvon.
  ASETUS = map(ASETUS, 0, 1023, 16000, 30000);    //Muuntaa potentiometrin äärilukemat 0 ja 1023, 16000 ja 30000 arvoisiksi (0 = 16000 ja 1023 = 30000). Näin tehtiin koska haluttiin potentiometrin säädölle enemmän "askellusta", jolloin säätämisen tarkkuus parani. Tässä haetaan 16 asteen minimiarvoa ja 30 asteen maksimiarvoa.
  Serial.print("Raja-arvo: ");
  Serial.print(ASETUS/1000.0);                    //ASETUS arvo jaetaan tuhannella joka aiemmin mäpättiin tuhatkertaiseksi lisätarkkuuden saavuttamiseksi. Lisättäessä piste ja nolla luvun perään, saadaan sarjaporttiin desimaalilukuja.
  Serial.println(" astetta");
}

void ohjaus(){
  ASTE*=1000;                                         //Kertoo luetun lämpötilan 1000 kertaisesti, koska haluttiin säätöön enemmän tarkkuutta.
  OHJAUS = map(ASTE - ASETUS, 1000, 7000, 60, 255);   //Jos mitatun arvon ja asetetun arvon erotus on esim 1000 (Alunperin 1 aste), mäppäytyy OHJAUS:ksen analogWrite arvoksi 60, jolloin puhallin alkaa pyöriä hiljaisimmalla mahdollisella nopeudella. 255 on vastaavasti suurin nopeus.
  #ifdef debug                                        //Jos debug ei ole käytössä define:issa, ohjelma jättää #ifdef - #endif välisen rivin väliin.
  Serial.println(OHJAUS);                           //Tulostaa ASTE - ASETUS erotuksen; käytetty ohjelman kehittämistä helpottaaksi.
  #endif debug 
  if(OHJAUS<0){                                     //Jos OHJAUS arvo on vähemmän kuin 0,
    analogWrite(FAN, 0);                            //puhallin pidetään poissa päältä.
  }else if(OHJAUS > 255){                           //Jos OHJAUS arvo on enemmän kuin 255,
    analogWrite(FAN, 255);                          //pyörii puhallin maksiminopeutta. Tämä lisättiin siksi että jos AD arvo nousi aiemmin yli 255, hidastui puhaltimen pyöriminen, koska arvo palasi takaisin 0:n. Nyt tämä lisää 255 + OHJAUS arvo.
  }else{
    analogWrite(FAN, OHJAUS);                       //Muutoin puhallin pyörii OHJAUS arvon mukaisella nopeudella.
  }
}

void valot(){
 if(ASTE > ASETUS){                //Jos mitattu arvo on enemmän kuin asetettu lämpötila,
    digitalWrite(LEDVIH, LEDOFF);   //vihreä ledi sammutetaan,
    digitalWrite(LEDSIN, LEDON);    //ja sininen ledi sytytetään.
  }else if(ASTE < ASETUS){          //Jos mitattu arvo on vähemmän kuin asetettu,
    digitalWrite(LEDSIN, LEDOFF);   //sammutetaan sininen ledi.
    digitalWrite(LEDVIH, LEDON);    //Vihreä ledi palaa aina kun lämpötila on halutun arvoinen; silloin kun on jäähdytetty tarpeeksi matalalle
  }
}
  
void loop() {
  keskiarvo();             //Funktiokutsut eri toiminnoille.
  mittaus();
  asetettu();
  delay(500);             //Viive jottei sarjaporttiin tulostuisi niin usein.
  ohjaus();
  valot();
 
 }

#include <Wire.h>//Nodig om te kunnen communiceren met de slaves
#include <Keypad.h>//Nodig om de keypad aan te kunnen sturen


//Keypad
//Dit is code om de keypad aan te kunnen sturen.
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
// r1  r2  r3  r4
///////////////////////////////////////////// !!!!!!!!!!!!!!!!!!!nog goed maken
byte rowPins[ROWS] = {2, 4, 3, 5};
byte colPins[COLS] = {6, 7, 8, 9};
// C4  c3  c2  c1

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//Motor
//Dit zijn de pins waar de motor op is aangesloten.
#define IN1  31
#define IN2  33
#define IN3  35
#define IN4  37

//Dit geeft aan hoe lang de code draait.
long int tijd;

//Met de volgende variabel geven we aan waar de lift is.
int positie;
//Deze waarde wordt verstuurd aan de slaves om de seven-segment display aan te kunnen sturen
int seven_segment_display ;

int aantal_liften = 5;

//is om te testen
int Input;

// Dit is de lijst van waardes die ontvangen wordt door de slaves. Het gaat hier om de sensor en de twee knoppen.
int sensor[] = {0, 0, 0, 0, 0};
int omhoog[] = {0, 0, 0, 0, 0};
int omlaag[] = {0, 0, 0, 0, 0};


//De volgende variabel geeft aan of de motor moet stoppen of niet.
int stoppen = 0;

boolean Direction;//Deze waarde geeft aan of de motor omhoog of omlaag moet draaien.
//De volgende waardes heb je nodig om de motor aan te kunnen sturen.
unsigned long last_time;
unsigned long currentMillis ;
int Steps;
long time;

//De volgende variabelen geven aan wat de bestemming is van de lift, en wat de afstand is tussen de positie en de bestemming van de lift.

int liftkooi_afstand_omhoog, bestemming_omhoog ;
int liftkooi_afstand_omlaag,  bestemming_omlaag ;
int liftkooi_afstand_key,  bestemming_key ;
//Dit geeft aan welk knop is ingedrukt.
int selected_Key;

void setup() {
  //De master zit op bus 1, en zet de serial monitor aan
  Wire.begin(1);
  Serial.begin(9600);

  //Hier geven we aan dat de motor pins een output is
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

}

void loop() {
//In de loop gaan we eerst naar de code onder gegevens, dan de code genaamd motor, en uiteindelijk wordt de tijd 1 hoger. 
//Nadat dit allemaal gedaan is loopt de code weer.
  /*
    Input = Serial.read() - '0';
    if (Serial.available() > 0) {
    if (Input == -49) {
    } if (Input == -38) {
    } else {
      Direction = Input;
    }

  */

  gegevens();
  motor();
  tijd++;

}


void stepper() {
  //Dit is code van de steppermotor. Hierbinnen draait de motor steeds een achtste door middel van switch cases.
  switch (Steps) {
    case 0:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;

    case 1:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, HIGH);
      break;
    case 2:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
    case 3:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      break;
    case 4:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
    case 5:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
    case 6:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
    case 7:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      break;

    default:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      break;
  }
  //Roept de code onder SetDirection aan.
  SetDirection();
}


void SetDirection() {
  //Hier wordt de juiste richting opgegeven voor de lift
  if (Direction == 1) {
    Steps++;
  }
  if (Direction == 0) {
    Steps--;
  }
  if (Steps > 7) {
    Steps = 0;
  }
  if (Steps < 0) {
    Steps = 7;
  }
}

void motor() {
  while (stoppen == 0) {
//Hier wordt afgelezen wat de afstand is naar de bestemming, en door middel daarvan de juiste richting ingesteld wordt.
    /*

      Serial.println(liftkooi_afstand_omhoog > 0 || liftkooi_afstand_omlaag > 0 || liftkooi_afstand_omlaag > 0);

      Serial.println(liftkooi_afstand_omhoog == 0 || liftkooi_afstand_omlaag == 0 || liftkooi_afstand_key == 0);
    */
    if (liftkooi_afstand_omhoog < 0 || liftkooi_afstand_omlaag < 0 || liftkooi_afstand_key < 0) {
      Direction = true  ;

    } else if (liftkooi_afstand_omhoog > 0 || liftkooi_afstand_omlaag > 0  || liftkooi_afstand_key < 0) {
      Direction = false;

    } else if (liftkooi_afstand_omhoog == 0 || liftkooi_afstand_omlaag == 0 || liftkooi_afstand_key == 0) {
      stoppen = 1;

    }


    //Dit is code voor de motor, met onder andere de snelheid//////////
    ///////////////////////////////////////////////////////////////////
    currentMillis = micros();//snelheid
    if (currentMillis - last_time > 1000) {
      stepper();
      time = time + micros() - last_time;
      last_time = micros();
      ///////////////////////////////////////////////////////////////////
    }
    //Code genaamd gegevens wordt aangeroepen
    gegevens();
  }
}


void gegevens() {
  //Met de volgende for-loop wordt de gegevens van alle slaves afgelezen. Onder gegevens verstaan we de sensor en de twee knoppen.
  for (int i = 0; i <= 4; i++) {
    int re_i = i + 2;
    Wire.requestFrom(re_i, 6);
    sensor[i] = Wire.read();
    omhoog[i] = Wire.read();
    omlaag[i] = Wire.read();


    /*
            Serial.print("etage lift ");
            Serial.print(sensor[i]);
            Serial.print(omhoog[i]);
            Serial.println(omlaag[i]);

    */

  }
  //Serial.println("///////////////////////////////////////////////////////////////////");



  //keypad



  //Met de volgende if-statements stellen we met behulp van de eerder aangegeven sensor waardes de positie en de seven-segment display in.
  if (sensor[0] == 0) {
    seven_segment_display = 0;
    positie = 1;
  } if (sensor[1] == 0) {
    seven_segment_display = 1;
    positie = 2;
  } if (sensor[2] == 0) {
    seven_segment_display = 2;
    positie = 3;
  } if (sensor[3] == 0) {
    seven_segment_display = 3;
    positie = 4;
  } if (sensor[4] == 0) {
    seven_segment_display = 4;
    positie = 5;
  }
  //Met de volgende if-statements stellen we met behulp van de eerder aangegeven omhoog knop waardes de bestemming voor de lift in.
  if ( omhoog[0] == 1) {
    bestemming_omhoog  = 1;
  } else   if ( omhoog[1] == 1) {
    bestemming_omhoog  = 2;
  }  else  if ( omhoog[2] == 1) {
    bestemming_omhoog  = 3;
  }  else  if ( omhoog[3] == 1) {
    bestemming_omhoog  = 4;
  }  else  if ( omhoog[4] == 1) {
    bestemming_omhoog  = 5;
  } else {
    bestemming_omhoog  = positie;
  }
  //Met de volgende if-statements stellen we met behulp van de eerder aangegeven omlaag knop waardes de bestemming voor de lift in.
  if (omlaag[0] == 1) {
    bestemming_omlaag  = 1;
  } else if (omlaag[1] == 1) {
    bestemming_omlaag  = 2;
  } else if (omlaag[2] == 1) {
    bestemming_omlaag  = 3;
  } else if (omlaag[3] == 1) {
    bestemming_omlaag  = 4;
  } else if (omlaag[4] == 1) {
    bestemming_omlaag  = 5;
  } else {
    bestemming_omlaag  = positie;
  }

  if (omlaag[0] == 1) {
    bestemming_omlaag  = 1;
  } else if (omlaag[1] == 1) {
    bestemming_omlaag  = 2;
  } else if (omlaag[2] == 1) {
    bestemming_omlaag  = 3;
  } else if (omlaag[3] == 1) {
    bestemming_omlaag  = 4;
  } else if (omlaag[4] == 1) {
    bestemming_omlaag  = 5;
  } else {
    bestemming_omlaag  = positie;
  }

  //Hier wordt de bestemming voor de lift ingesteld zodra er een knop wordt ingedrukt. 
  //Als er nog niks is ingedrukt is de bestemming dezelfde als de positie.
  int key = customKeypad.getKey() - '0';
  bestemming_key = positie;

  if (key != -48) {
    selected_Key = key;
  } else if (selected_Key != key) {
    if (selected_Key == 1) {
      bestemming_key  = 1;
    } else if (selected_Key == 2) {
      bestemming_key  = 2;
    } else if (selected_Key == 3) {
      bestemming_key  = 3;
    } else if (selected_Key == 4) {
      bestemming_key  = 4;
    } else if (selected_Key == 5) {
      bestemming_key  = 5;
    }
  } else {
    selected_Key = -48;
  }



  //Met de volgende for-loop schrijven we de waarde van de seven-segment display aan alle slaves.
  for (int t = 2; t <= 6; t++) {
    Wire.beginTransmission(t);
    Wire.write(seven_segment_display);
    Wire.endTransmission();
  }



  //Hiermee hebben we berekend of de lift naar boven of naar beneden moet. Als de uitkomst 0 is staat hij stil, als de uitkomst positief is gaat hij omhoog,
  //en als de uitkomst negatief is gaat hij omlaag.
  liftkooi_afstand_omhoog = bestemming_omhoog - positie;
  liftkooi_afstand_omlaag = bestemming_omlaag - positie;

  liftkooi_afstand_key = bestemming_key - positie;

  if ( liftkooi_afstand_omhoog != 0 || liftkooi_afstand_omlaag != 0 || liftkooi_afstand_key != 0) {
    stoppen = 0;
  }


  /*
    Serial.print("bestemming_omlaag ");
    Serial.println(bestemming_omlaag);

    Serial.print("bestemming_omhoog ");
    Serial.println(bestemming_omhoog);


    Serial.print("positie ");
    Serial.println(positie);
  */
/*
  Serial.print("liftkooi_afstand_omhoog ");
  Serial.println(liftkooi_afstand_omhoog);

  Serial.print("liftkooi_afstand_omlaag ");
  Serial.println(liftkooi_afstand_omlaag);


  Serial.print("liftkooi_afstand_key ");
  Serial.println(liftkooi_afstand_key);
*/
  /*
    Serial.print("positie ");
    Serial.println(positie);

    Serial.print("bestemming_key ");
    Serial.println(bestemming_key);


    Serial.print("key ");
    Serial.println(key);
  */
}

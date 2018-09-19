/*
  Microscope controller for lighting system
  by Daniel Dominguez

  This is a simple program that uses three nobs and three
  fets to get fine control of the lights.

  a switch loop will be used to handle the proper parts of the program
  a function to read all the potentimeter will be useful
  I hooked up the button to pin 2 that I could use the interrupt
  to call an interrupt service routine to change the case of the switch loop.

  Apologies for viloating pep8, I am still sloppy
*/

int mid_three_out = 9;    // controls middle fet @3.3v
int rght_three_out = 10;  // controls right fet @3.3v
int twelve_out = 6;       // controls Left fet @12v

int m_pot_in = A1;       // middle potentimeter (10k)
int r_pot_in = A0;       // right potenimterter
int l_pot_in = A2;       // left potentimeter

int button1 = 2;    // hook up to the button

int led = 13; // using the built in Led to give status

int r = 0;
int l = 0;
int m = 0;

int event = 0;

unsigned long currentMillis = 0; //start the time at zero
unsigned long previousMillis = 0;
const long interval = 333;
int ledState = LOW;

void read_pots() {   // function to get the current pot values
  r = map(analogRead(r_pot_in), 0, 1023, 0 , 255);
  l = map(analogRead(l_pot_in), 0, 1023, 0, 255);
  m = map(analogRead(m_pot_in), 0, 1023, 0 , 255);
}

void triplet_control(int x) {  // set the state of all the fets at once
  analogWrite(mid_three_out, x);
  analogWrite(rght_three_out, x);
  analogWrite(twelve_out, x);
}

void light_control(int x, int y, int z) {  // set the state of all the fets at once
  analogWrite(mid_three_out, y);
  analogWrite(rght_three_out, z);
  analogWrite(twelve_out, x);
}

void led_state() {
  currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(led, ledState);
    }
}
void button() {
  if (digitalRead(button1) == false) {
    event += 1;
    if (event == 3) { // == number of toggle states -i.e. (0,1)
      event = 0;
    }
    delay(150);
  }
}

void setup() {
  //Initialize all the inputs and outputs
  pinMode(mid_three_out, OUTPUT);
  pinMode(rght_three_out, OUTPUT);
  pinMode(twelve_out, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(m_pot_in, INPUT);
  pinMode(r_pot_in, INPUT);
  pinMode(l_pot_in, INPUT);
  // pull the button pin high, and attach it to interrupt on falling from hi to low
  pinMode(button1, INPUT_PULLUP);
  //Serial.begin(9600);
}

void loop() {
  switch (event) {
    case 0:
      button();
      triplet_control(0);
      digitalWrite(led, LOW);
      break;
    case 2:
      button();
      triplet_control(1023);
      digitalWrite(led, HIGH);
      break;
    case 1:
      button();
      read_pots();
      light_control(r, m, l);
      led_state();
      //Serial.println(r);
      break;
  }
}




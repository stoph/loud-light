const int sampleWindow = 750; // Sample window width in ms
unsigned int sample;

int relay = D2;
int mic = A0;
int pin, pinOff1, pinOff2;

void setup() {
  Serial.begin(9600);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
}

/*
 * Wemos D1 mini pins connected to relay board
 * 
 * D4 = CH2 (Red)
 * D3 = CH3 (Yellow)
 * D2 = CH4 (Green)
 */
void light(const char * color, const char * state, boolean exclusive = true) {
  if (color == "green") {
      pin = D2;
      pinOff1 = D3;
      pinOff2 = D4;
  } else if (color == "yellow") {
      pin = D3;
      pinOff1 = D2;
      pinOff2 = D4;
  } else if (color == "red") {
      pin = D4;
      pinOff1 = D2;
      pinOff2 = D3;
  }
  
  if (state == "on") {
    digitalWrite(pin, LOW);
    if (exclusive) {
      digitalWrite(pinOff1, HIGH);
      digitalWrite(pinOff2, HIGH);
    }
  } else {
    digitalWrite(pin, HIGH);
  }
}

void loop() {
  
  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow) {
  sample = analogRead(mic);
    if (sample < 1024) { // toss out spurious readings
      if (sample > signalMax) {
        signalMax = sample;  // save just the max levels
      } else if (sample < signalMin) {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024;  // convert to volts

  if (volts <= 2.5) {
    light("green", "on");
  } else if (volts <= 4.5) {
    light("yellow", "on");
  } else {
    light("red", "on");
  }
  
  // Serial.println(volts);
}

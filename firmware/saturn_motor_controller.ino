// Firmware for Arduino: controls a DC motor (PWM) and NeoPixel ring
// Commands over Serial (from backend or USB-Serial monitor):
// START               -> start motor and default light
// STOP                -> stop motor
// SPEED:<0-255>       -> set motor PWM speed
// COLOR:R,G,B         -> set solid color for LED
// PATTERN:romantic     -> enable romantic pulsing pattern
// MSG:<text>          -> trigger short light pattern for message

#include <Adafruit_NeoPixel.h>

#define PIN_NEOPIXEL 6
#define NUMPIXELS 16

#define MOTOR_PWM_PIN 5
#define MOTOR_DIR_PIN 4

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

bool motorRunning = false;
int motorSpeed = 150; // 0-255

enum Pattern { NONE, ROMANTIC } currentPattern = NONE;

uint8_t colorR = 255, colorG = 100, colorB = 50; // default saturn-ish

unsigned long lastMillis = 0;
int pulseDir = 1;
int pulseVal = 0;

String inputBuffer = "";

void setup() {
  Serial.begin(115200);
  pixels.begin();
  pixels.show();

  pinMode(MOTOR_PWM_PIN, OUTPUT);
  pinMode(MOTOR_DIR_PIN, OUTPUT);
  digitalWrite(MOTOR_DIR_PIN, HIGH); // set direction
  analogWrite(MOTOR_PWM_PIN, 0);

  setSolidColor(colorR, colorG, colorB);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        handleCommand(inputBuffer);
        inputBuffer = "";
      }
    } else {
      inputBuffer += c;
    }
  }

  if (currentPattern == ROMANTIC) {
    runRomanticPulse();
  }

  // Motor control
  if (motorRunning) {
    analogWrite(MOTOR_PWM_PIN, motorSpeed);
  } else {
    analogWrite(MOTOR_PWM_PIN, 0);
  }
}

void handleCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "START") {
    motorRunning = true;
    currentPattern = ROMANTIC;
    Serial.println("OK:START");
    return;
  }
  if (cmd == "STOP") {
    motorRunning = false;
    currentPattern = NONE;
    setSolidColor(colorR, colorG, colorB);
    Serial.println("OK:STOP");
    return;
  }

  if (cmd.startsWith("SPEED:")) {
    String val = cmd.substring(6);
    int s = val.toInt();
    s = constrain(s, 0, 255);
    motorSpeed = s;
    Serial.print("OK:SPEED="); Serial.println(motorSpeed);
    return;
  }

  if (cmd.startsWith("COLOR:")) {
    String vals = cmd.substring(6);
    int comma1 = vals.indexOf(',');
    int comma2 = vals.indexOf(',', comma1 + 1);
    if (comma1 > 0 && comma2 > 0) {
      int r = vals.substring(0, comma1).toInt();
      int g = vals.substring(comma1 + 1, comma2).toInt();
      int b = vals.substring(comma2 + 1).toInt();
      colorR = constrain(r, 0, 255);
      colorG = constrain(g, 0, 255);
      colorB = constrain(b, 0, 255);
      setSolidColor(colorR, colorG, colorB);
      Serial.println("OK:COLOR");
    }
    return;
  }

  if (cmd.startsWith("PATTERN:")) {
    String p = cmd.substring(8);
    if (p == "ROMANTIC") {
      currentPattern = ROMANTIC;
      Serial.println("OK:PATTERN=ROMANTIC");
    } else {
      currentPattern = NONE;
      Serial.println("OK:PATTERN=NONE");
    }
    return;
  }

  if (cmd.startsWith("MSG:")) {
    // For simple effect: quick color flash sequence
    String m = cmd.substring(4);
    flashMessagePattern(m);
    Serial.println("OK:MSG");
    return;
  }

  Serial.println("ERR:UNKNOWN");
}

void setSolidColor(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
}

void runRomanticPulse() {
  unsigned long now = millis();
  if (now - lastMillis < 20) return; // update ~50Hz
  lastMillis = now;

  pulseVal += pulseDir * 2; // speed of pulsing
  if (pulseVal >= 200) { pulseVal = 200; pulseDir = -1; }
  if (pulseVal <= 30) { pulseVal = 30; pulseDir = 1; }

  int r = (colorR * pulseVal) / 255;
  int g = (colorG * pulseVal) / 255;
  int b = (colorB * pulseVal) / 255;

  setSolidColor(r, g, b);
}

void flashMessagePattern(String m) {
  // Quick three pulses in warm pink for messages
  for (int t = 0; t < 3; t++) {
    setSolidColor(255, 100, 180);
    delay(250);
    setSolidColor(0, 0, 0);
    delay(150);
  }
  // restore
  setSolidColor(colorR, colorG, colorB);
}

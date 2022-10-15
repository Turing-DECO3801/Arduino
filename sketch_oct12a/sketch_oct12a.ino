#include <Adafruit_NeoPixel.h>

#define BUTTON_COLOUR   32

#define PIXEL_PIN    27

#define BUTTON_ON    15

#define PIXEL_COUNT 16 // 16 is our number but for testing neopixel 12

#define TOGGLE 0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int showType = 0;
int hold = 0;

bool lastButtonState = HIGH;
bool buttonStatePrevious = HIGH;
bool changeColourPrevious = HIGH;
bool pixelState = LOW;

int toggle = 0;

int colour = 0;

int pauseColour = 0;

bool turnedOn = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_ON, INPUT_PULLUP);
  pinMode(BUTTON_COLOUR, INPUT_PULLUP);
  pinMode(PIXEL_PIN, OUTPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

int lastAudioButtonPress = 0;

int lastOnOffButtonPress = 0;

int temp = millis();

void loop() {
  /*
  bool test1 = digitalRead(BUTTON_ON);
  bool test2 = digitalRead(BUTTON_COLOUR);
  Serial.println("-------------");
  Serial.println(test1);
  Serial.println(test2);
  return;
  */

  // Buttons stay low and will go high on button press

  int currentTime = millis();

  bool buttonStateCurrent = digitalRead(BUTTON_ON);
  if (currentTime > lastOnOffButtonPress + 500 && buttonStateCurrent == HIGH) {// && buttonStatePrevious == LOW) {
    if (colour == 0) {
      play(strip.Color(0, 0, 0)); // off
      turnedOn = LOW;
    } else {
      play(strip.Color(255, 255, 255)); // on
      turnedOn = HIGH;
    }
    colour = (colour + 1) % 2;
    lastOnOffButtonPress = millis();
  }
  // buttonStatePrevious = buttonStateCurrent;


  bool changeColourCurrent = digitalRead(BUTTON_COLOUR);
  if (turnedOn == HIGH && changeColourCurrent == HIGH) {
    Serial.println("PRESS START");
    // if (BUTTON_COLOUR == LOW) {
      
    // }
    // if ((currentTime - temp) > 2000) {

    // }
    int startHoldTime = millis();
    bool completed = false;
    // Hold. the button until 500ms has passed or button is released
    while (changeColourCurrent == HIGH) {
      changeColourCurrent = digitalRead(BUTTON_COLOUR);
      if (millis() > startHoldTime + 1000) {
        completed = true;
        break;
      }
    }

    // The button was held down the entire duration
    if (completed) {
        play(strip.Color(255, 255, 255)); // white
        pauseColour = 0;
    } else {
      if (pauseColour == 0) {
        play(strip.Color(0, 255, 0)); // green
      } else {
        play(strip.Color(255, 165, 00)); // orange
      }
      pauseColour = (pauseColour + 1) % 2;
    }

    changeColourCurrent = digitalRead(BUTTON_COLOUR);
    while (changeColourCurrent == HIGH) {
      changeColourCurrent = digitalRead(BUTTON_COLOUR);
      Serial.println("HOLDING");
    }

    // if (currentTime > lastAudioButtonPress + 500 && changeColourCurrent == LOW && changeColourPrevious == HIGH) {
    //   if (pauseColour == 0) {
    //     play(strip.Color(255, 0, 0)); // red
    //   } else {
    //     play(strip.Color(0, 0, 255)); // blue
    //   }
    //   pauseColour = (pauseColour + 1) % 2;
    //   lastAudioButtonPress = millis();
    // }
  }
  // changeColourPrevious = changeColourCurrent;
}

void play(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show(); 
    //delay(5);
  }
  
}



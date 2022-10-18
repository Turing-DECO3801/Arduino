#include <Adafruit_NeoPixel.h>

#define BUTTON_COLOUR   32

#define PIXEL_PIN    27

#define BUTTON_ON    15

#define PIXEL_COUNT 16 // 16 is our number but for testing neopixel 12

#define TOGGLE 0

// AdaFruit NeoPixel Library for lighting control of the NeoPixel ring
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

  // Initialisation of READ and WRITE pins on the ESP32
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
  // Buttons stay low and will go high on button press

  int currentTime = millis();

  // Checks if the START/STOP button has been pressed
  bool buttonStateCurrent = digitalRead(BUTTON_ON);
  
  // To prevent debouncing, a 0.5s delay is required and a high must be received
  if (currentTime > lastOnOffButtonPress + 500 && buttonStateCurrent == HIGH) {

    // Alternate the colour based on current colour
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

  // Checks if the RECORD/STOP RECORDING button has been pressed
  bool changeColourCurrent = digitalRead(BUTTON_COLOUR);
  if (turnedOn == HIGH && changeColourCurrent == HIGH) {
    Serial.println("PRESS START");

    // Checks the time that the hold had started
    int startHoldTime = millis();

    // Flag to check if the 1 second hold was completed
    bool completed = false;

    // Hold the button until 500ms has passed or button is released
    while (changeColourCurrent == HIGH) {
      changeColourCurrent = digitalRead(BUTTON_COLOUR);

      // Hold has occurred for 1 second and should now change colour
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
  }
}


/**
 * Updates the RGB value on the lighting strip for 
 */
void play(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show(); 
  }
  
}



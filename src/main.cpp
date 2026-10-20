#include <Arduino.h>
#include <TFT_eSPI.h>

//LED VARIABLES
const int LED_PIN_R=14;
const int LED_PIN_B=26;
const int LED_PIN_G=27;
bool cycling = true;
int step = 0;
unsigned long lastChange = 0;
int level=0;

//JS variables
const int JS_X_PIN=36;
const int JS_Y_PIN=39;
const int JS_SW_PIN=32;
int x_val=0;
int y_val=0;
bool sw_pressed=0;

//display variables

TFT_eSPI tft = TFT_eSPI();
const int CELL = 20;
const uint16_t GRID_COLOR = TFT_DARKGREY;
int width, height, columns, rows;

//pot variable
const int POT_PIN = 33;

void readPot(){

  int reading = analogRead(POT_PIN);
  Serial.printf("Pot reading: %d\n",reading);
}



//code for led cycling
void setupLEDs(){

  //pwm setup
  ledcSetup(0, 3000, 8);  
  ledcAttachPin(LED_PIN_R, 0);
  ledcSetup(1, 3000, 8);  
  ledcAttachPin(LED_PIN_B, 1);
  ledcSetup(2, 3000, 8);  
  ledcAttachPin(LED_PIN_G, 2);


}

void setColour(uint8_t red, uint8_t blue, uint8_t green){

  ledcWrite(0, 255-red);
  ledcWrite(1, 255-blue);
  ledcWrite(2, 255-green);

}

void loopRBG(){

  if (cycling && millis() - lastChange >= 5) {
    lastChange = millis();

    level += 5;
    if (level > 255) {
      level = 0;
      step = (step + 1) % 3;
    }

    if (step == 0) setColour(level, 0, 0);
    if (step == 1) setColour(0, level, 0);
    if (step == 2) setColour(0, 0, level);
  }
}

//read joystick
void joystickRead(){

  x_val=analogRead(JS_X_PIN);
  y_val=analogRead(JS_Y_PIN);
  sw_pressed=(digitalRead(JS_SW_PIN)==LOW);

  //print values to serial
  Serial.printf("X= %d, Y=%d\n", x_val, y_val);

  if (sw_pressed){

    Serial.printf("Joystick Button is pressed\n");

  }

  delay(100);

}

//lcd functions
void drawGrid() {

  for (int x = 0; x <= width; x += CELL) tft.drawFastVLine(x, 0, height, GRID_COLOR);
  for (int y = 0; y <= height; y += CELL) tft.drawFastHLine(0, y, width, GRID_COLOR);

}

void drawNumbers(){
  //in each cell, draw a random number
  tft.setTextColor(TFT_CYAN);   
  tft.setTextDatum(MC_DATUM);

  for(int r=0;r < rows; r++){

    for(int c=0; c < columns; c++){
      int cx = c * CELL + CELL / 2;   
      int cy = r * CELL + CELL / 2;
      tft.drawNumber(random(1,4), cx, cy, 1);

    }
  }


}

void setup() {
  Serial.begin(115200);
  pinMode(JS_SW_PIN, INPUT_PULLUP);
  setupLEDs();
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  width = tft.width();
  height = tft.height();
  columns = width / CELL;
  rows = height / CELL;

  drawGrid();
  drawNumbers();

  analogReadResolution(12);
  analogSetPinAttenuation(POT_PIN, ADC_11db);


}

 void loop() {
  loopRBG();
  joystickRead();
  readPot();
  //delay(100);
 }
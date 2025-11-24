#include "Arduino_LED_Matrix.h" // led matrix library 

ArduinoLEDMatrix matrix; 

// initialize pins for analog ports
const pin_size_t potPinFreq = A0;
const pin_size_t potPinPhase = A1;
const pin_size_t potPinAmp = A2;

// declare variables to later be controlled by the potentiometer
float frequency;
float amplitude;
float phaseShift;

// initial phaseShift to start wave at 0
float offset = 0.25;

// led matrix x and y
uint8_t x;
uint8_t y;

// led matrix
uint8_t frame[8][12];

// utility math functions
float lerp(float outMin, float outMax, float alpha);
float inverseLerp(float value, float inMin, float inMax);
float remap(float value, float inMin, float inMax, float outMin, float outMax);

void turnEntireFrameOff(); 
void turnLEDsOn(int x, int y); 

void setup() {
  Serial.begin(115200); // baud value for led matrix library
  matrix.begin(); // needed to write to the led matrix
}
  
void loop(){
  // Reading and remapping values controlled by potentiometer
  frequency = remap(analogRead(potPinFreq), 0.0, 1023.0, 0.125, 2.5);
  phaseShift = remap(analogRead(potPinPhase), 0.0, 1023.0, 0.125, 2.0);
  amplitude = remap(analogRead(potPinAmp), 0.0, 1023.0, 0.0, 7.0);
  
  // render led matrix
  matrix.renderBitmap(frame, 8, 12);

  // convert time to seconds
  float time = millis() / 1000.0; // time in seconds

  // prepare new frame for leed amtrix
  turnEntireFrameOff();

  // Writing values for sin wave
  // Going through all leds in the matrix, which consits of 12 columns and 8 rows
  for(int i = 0; i < 12; i++){
    // 
    float sinValue = sinf(6.28 * frequency * i / 11 - 1.57 + 6.28 * time * phaseShift); // sinf returns -1.0 to 1.0
    float sinRemap = remap(sinValue, -1.0, 1.0, 0.0, amplitude); // sin remapped to 7.0 to 0.0

    x = i;
    y = (int)round(sinRemap);

    turnLEDsOn(y, x);
  }

  // Debugging purposes.
  Serial.print(time);
  Serial.print(',');
  Serial.print(frequency);
  Serial.print(',');
  Serial.print(phaseShift);
  Serial.print(',');
  Serial.print(x);
  Serial.print(',');
  Serial.println(y);
}

float lerp(float outMin, float outMax, float alpha) {
  // mixes 2 values linearly based on 0-1 range
  return outMin + (outMax - outMin) * alpha;
}

float inverseLerp(float value, float inMin, float inMax) {
  // makes value fit in a new 0-1 range by stretching or squaching linearly. It's like a spring that is eighter pushed or pulled
  return (value - inMin) / (inMax - inMin); 
}

float remap(float value, float inMin, float inMax, float outMin, float outMax) {
  // combine lerp, clamp/contrain and inverselerp to stretch one range, that we define, to another
  float alpha = inverseLerp(value, inMin, inMax);
  alpha = constrain(alpha, 0.0, 1.0);
  return lerp(outMin, outMax, alpha);
}

void turnEntireFrameOff(){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 12; j++){
      frame[i][j] = 0;
    }
  }
}

void turnLEDsOn(int x, int y){
  frame[x][y] = 1;
}






#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;
#define 

const pin_size_t potPinFreq = A0;
const pin_size_t potPinPhase = A1;
const pin_size_t potPinAmp = A2;

float freq;
float amp;
float phaseShift;
float offset = 0.25;

uint8_t x;
uint8_t y;

uint8_t frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

float lerp(float outMin, float outMax, float alpha);
float inverseLerp(float value, float inMin, float inMax);
float remap(float value, float inMin, float inMax, float outMin, float outMax);
void turnEntireFrameOff();
void turnLEDsOn(int x, int y);

void setup() {
  Serial.begin(115200); // baud value for led matrix library
  matrix.begin();
}
  
void loop(){
  // Reading vaules
  freq = remap(analogRead(potPinFreq), 0.0, 1023.0, 0.125, 2.5);
  phaseShift = remap(analogRead(potPinPhase), 0.0, 1023.0, 0.125, 2.0);
  amp = remap(analogRead(potPinAmp), 0.0, 1023.0, 0.0, 7.0);
  
  matrix.renderBitmap(frame, 8, 12);

  float time = millis() / 1000.0; // time in seconds

  turnEntireFrameOff();

  // Writing values
  for(int i = 0; i < 12; i++){
    float sinValue = sinf(6.28 * freq * i / 11 - 1.57 + 6.28 * time * phaseShift); // sinf returns -1.0 to 1.0
    float sinRemap = remap(sinValue, -1.0, 1.0, 0.0, amp); // sin remapped to 7.0 to 0.0

    x = i;
    y = (int)round(sinRemap);

    turnLEDsOn(y, x);
  }

  // Debugging purposes.
  Serial.print(time);
  Serial.print(',');
  Serial.print(freq);
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






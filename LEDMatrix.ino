#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

void setup() {
  Serial.begin(115200);
  matrix.begin();
}

// MOLSTEDS CODE!!

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

uint8_t isFrameOn = 0;

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

void turnEntireFrameOff(){
  //if(isFrameOn == 1) isFrameOn = 0;
  //else isFrameOn = 1;

  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 12; j++){
      frame[i][j] = 0;
    }
  }
}

void turnLEDsOn(int x, int y){
  //turnEntireFrameOff();
  frame[x][y] = 1;

}

float freq = 1.0;
float offset = 0.25;

float timeScale = 1.0;

int x;
int y;
  
void loop(){
  matrix.renderBitmap(frame, 8, 12);
  //turnEntireFrame();
  //delay(100);

  //delay(20);

  float time = millis() / 1000.0 * timeScale; // time in seconds


  turnEntireFrameOff();

  for(int i = 0; i < 12; i++){
    // float sinValue = sinf(6.28 * time * freq + (freq * i) - 1.57 + 6.28 * offset); // sinf returns -1.0 to 1.0
    float sinValue = sinf(6.28 * (time + i / (11.0 / timeScale)) * freq - 1.57 + 6.28 * offset); // sinf returns -1.0 to 1.0
    float sinRemap = remap(sinValue, -1.0, 1.0, 0.0, 8.0); // sin remapped to 0.0 to 8.0
    

    x = i;
    y = (int)sinRemap;

    turnLEDsOn(y, x);
  }


  Serial.print(time);
  Serial.print(',');
  Serial.print(x);
  Serial.print(',');
  Serial.println(y);
}





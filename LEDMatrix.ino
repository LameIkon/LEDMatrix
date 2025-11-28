#include "Arduino_LED_Matrix.h" // include led matrix library 

ArduinoLEDMatrix matrix; // LED Matrix object

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
  Serial.begin(115200); // opens serial port and sets data rate to baud value needed for LED matrix
  matrix.begin(); // start LED matrix
}
  
void loop(){
  // Reading analog values controlled by the potentiometer and remap 0 to 1023 to fit range for sin wave calculations
  frequency = remap(analogRead(potPinFreq), 0.0, 1023.0, 0.125, 2.5); // frequency range: 0.125 to 2.5
  phaseShift = remap(analogRead(potPinPhase), 0.0, 1023.0, 0.125, 2.0);
  amplitude = remap(analogRead(potPinAmp), 0.0, 1023.0, 0.0, 7.0);
  
  // render led matrix
  matrix.renderBitmap(frame, 8, 12);

  // time since program start in seconds
  float time = millis() / 1000.0;

  // prepare new frame for LED matrix update
  turnEntireFrameOff();

  // Write sin wave values to LED matrix,
  // by going through all 12 columns of the LED matrix and finding the belonging row according to the sin wave calculations
  for(int i = 0; i < 12; i++){
    // 6.28 (2 pi) is used to express a full wave per x, which makes further changes easier to calculate
    // We can then make the full length of the LED matrix represent 1 wave by letting each column, i, be 1 / 11 of a full wave
    // This way the frequency controls waves per full LED matrix length

    // The other part controls the phaseShift of the sin wave, 
    // with an initial offset of 1/2 pi to start at the bottom of the wave
    // The sin wave is animated by multiplying phaseShift with time, 
    // where phaseShift is waves per second, because the time is scaled with 2 pi
    
    float sinValue = sinf(6.28 * frequency * i / 11 - 1.57 + 6.28 * time * phaseShift); // sinf returns -1.0 to 1.0
    float sinRemap = remap(sinValue, -1.0, 1.0, 0.0, amplitude); // sin value remapped to 0.0 to 7.0 (amplitude)

    // set current x and y
    x = i;
    y = (int)round(sinRemap);

    // write and turn on the currenty looked at LED in the LED matrix
    turnLEDsOn(y, x);
    
    // repeat as long as there are more columns to look at in the LED matrix
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






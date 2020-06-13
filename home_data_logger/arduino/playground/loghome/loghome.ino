// Demo program for testing library and board - flip the switch to turn on/off buzzer

#include <Adafruit_CircuitPlayground.h>
#include "mean_var_data.h"

// we light one pixel at a time, this is our counter
uint8_t pixeln = 0;

MeanVarData* mean_var_temp = new MeanVarData(50);

void setup() {
  //while (!Serial);
  Serial.begin(9600);
  Serial.println("Circuit Playground test!");
  CircuitPlayground.begin();
}


void loop() {
  double sound_pressure = CircuitPlayground.mic.soundPressureLevel(5);
  // test Red #13 LED
  if (sound_pressure > 62)
  {
    CircuitPlayground.redLED(HIGH);
  }
  else
  {
    CircuitPlayground.redLED(LOW);
  }

  //  /************* TEST CAPTOUCH */
  //  Serial.print("Capsense #3: "); Serial.println(CircuitPlayground.readCap(3));
  //  Serial.print("Capsense #2: "); Serial.println(CircuitPlayground.readCap(2));
  //  if (! CircuitPlayground.isExpress()) {  // CPX does not have this captouch pin
  //    Serial.print("Capsense #0: "); Serial.println(CircuitPlayground.readCap(0));
  //  }
  //  Serial.print("Capsense #1: "); Serial.println(CircuitPlayground.readCap(1));
  //  Serial.print("Capsense #12: "); Serial.println(CircuitPlayground.readCap(12));
  //  Serial.print("Capsense #6: "); Serial.println(CircuitPlayground.readCap(6));
  //  Serial.print("Capsense #9: "); Serial.println(CircuitPlayground.readCap(9));
  //  Serial.print("Capsense #10: "); Serial.println(CircuitPlayground.readCap(10));
  //
  //  /************* TEST SLIDE SWITCH */
  //  if (CircuitPlayground.slideSwitch()) {
  //    Serial.println("Slide to the left");
  //  } else {
  //    Serial.println("Slide to the right");
  //    CircuitPlayground.playTone(500 + pixeln * 500, 100);
  //  }
  //
  //  /************* TEST 10 NEOPIXELS */
  //  CircuitPlayground.setPixelColor(pixeln++, CircuitPlayground.colorWheel(25 * pixeln));
  //  if (pixeln == 11) {
  //    pixeln = 0;
  //    CircuitPlayground.clearPixels();
  //  }
  if (CircuitPlayground.lightSensor() > 20)
  {
    if (sound_pressure > 52)
    {
      pixeln = 0;
      CircuitPlayground.setPixelColor(pixeln, CircuitPlayground.colorWheel(25 * pixeln));
    }
    else
    {
      pixeln = 0;
      CircuitPlayground.setPixelColor(pixeln, 0, 0, 0);
    }

    if (sound_pressure > 58)
    {
      pixeln = 1;
      CircuitPlayground.setPixelColor(pixeln, CircuitPlayground.colorWheel(25 * pixeln));
    }
    else
    {
      pixeln = 1;
      CircuitPlayground.setPixelColor(pixeln, 0, 0, 0);
    }

    if (sound_pressure > 62)
    {
      pixeln = 2;
      CircuitPlayground.setPixelColor(pixeln, CircuitPlayground.colorWheel(25 * pixeln));
    }
    else
    {
      pixeln = 2;
      CircuitPlayground.setPixelColor(pixeln, 0, 0, 0);
    }

    if (sound_pressure > 64)
    {
      pixeln = 3;
      CircuitPlayground.setPixelColor(pixeln, CircuitPlayground.colorWheel(25 * pixeln));
    }
    else
    {
      pixeln = 3;
      CircuitPlayground.setPixelColor(pixeln, 0, 0, 0);
    }


    if (sound_pressure > 66)
    {
      pixeln = 4;
      CircuitPlayground.setPixelColor(pixeln, CircuitPlayground.colorWheel(25 * pixeln));
    }
    else
    {
      pixeln = 4;
      CircuitPlayground.setPixelColor(pixeln, 0, 0, 0);
    }


    if (sound_pressure > 68)
    {
      pixeln = 5;
      CircuitPlayground.setPixelColor(pixeln, CircuitPlayground.colorWheel(25 * pixeln));
    }
    else
    {
      pixeln = 5;
      CircuitPlayground.setPixelColor(pixeln, 0, 0, 0);
    }

    if (sound_pressure > 70)
    {
      pixeln = 6;
      CircuitPlayground.setPixelColor(pixeln, CircuitPlayground.colorWheel(25 * pixeln));
    }
    else
    {
      pixeln = 6;
      CircuitPlayground.setPixelColor(pixeln, 0, 0, 0);
    }

    if (sound_pressure > 72)
    {
      pixeln = 7;
      CircuitPlayground.setPixelColor(pixeln, CircuitPlayground.colorWheel(25 * pixeln));
    }
    else
    {
      pixeln = 7;
      CircuitPlayground.setPixelColor(pixeln, 0, 0, 0);
    }

    if (sound_pressure > 75)
    {
      pixeln = 8;
      CircuitPlayground.setPixelColor(pixeln, CircuitPlayground.colorWheel(25 * pixeln));
    }
    else
    {
      pixeln = 8;
      CircuitPlayground.setPixelColor(pixeln, 0, 0, 0);
    }

    if (sound_pressure > 80)
    {
      pixeln = 9;
      CircuitPlayground.setPixelColor(pixeln, CircuitPlayground.colorWheel(25 * pixeln));
    }
    else
    {
      pixeln = 9;
      CircuitPlayground.setPixelColor(pixeln, 0, 0, 0);
    }
  }
  else
  {
    CircuitPlayground.clearPixels();
  }
  /************* TEST BOTH BUTTONS */
  if (CircuitPlayground.leftButton()) {
    Serial.println("Left button pressed!");
  }
  if (CircuitPlayground.rightButton()) {
    Serial.println("Right button pressed!");
  }

  /************* TEST LIGHT SENSOR */
  //Serial.print("Light sensor: ");
  Serial.print(CircuitPlayground.lightSensor());

  /************* TEST SOUND SENSOR */
  Serial.print(",");
  Serial.print(sound_pressure);

  //  /************* TEST ACCEL */
  //  // Display the results (acceleration is measured in m/s*s)
  //Serial.print("X: ");
  Serial.print(",");
  Serial.print(CircuitPlayground.motionX());
  //Serial.print(" \tY: ");
  Serial.print(",");
  Serial.print(CircuitPlayground.motionY());
  //Serial.print(" \tZ: ");
  Serial.print(",");
  Serial.print(CircuitPlayground.motionZ());
  //Serial.println(" m/s^2");

  /************* TEST THERMISTOR */
  Serial.print(",");
  double temp_temp = CircuitPlayground.temperature();
  calculateMeanAndVar(temp_temp, mean_var_temp);
  //Serial.println(temp_temp);
  Serial.print(mean_var_temp->mean);
  Serial.print(":");
  Serial.println(mean_var_temp->var);

  delay(100);
}

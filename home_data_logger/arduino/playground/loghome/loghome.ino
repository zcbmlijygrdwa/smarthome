// Demo program for testing library and board - flip the switch to turn on/off buzzer

#include <Adafruit_CircuitPlayground.h>
#include "mean_var_data.h"
#include "common_types.h"
#include "led_gauge.h"
#include "gauge.h"
#include "car_motion.h"

long counter = 0;

// we light one pixel at a time, this is our counter
uint8_t pixeln = 0;

CarMotion car_motion;
bool car_motion_alarm_triggered = false;

Gauge gauge_acc(10,0,10);
Gauge gauge_car_motion(10,0,10);
Gauge gauge_light(10,0,500);
Gauge gauge_sound(10,50,65);
int display_mode_number = 4;
int display_mode = 0;

SensorInfo sensor_info;


void updateSensorInfo(SensorInfo& sensor_info)
{
    /************* TEST BOTH BUTTONS */
    
    sensor_info.button_sensor_info.left_button_pressed = CircuitPlayground.rightButton();
    sensor_info.button_sensor_info.right_button_pressed = CircuitPlayground.leftButton();

    sensor_info.light_sensor_info.setData(CircuitPlayground.lightSensor());
    
    sensor_info.temperature_sensor_info.setData(CircuitPlayground.temperature());
    

    
    sensor_info.accelerometer_sensor_info.setData(CircuitPlayground.motionX(), CircuitPlayground.motionY(), CircuitPlayground.motionZ());

    double sound_pressure = CircuitPlayground.mic.soundPressureLevel(2);
    //double sound_pressure = CircuitPlayground.mic.soundPressureLevel(5);
    //double sound_pressure = CircuitPlayground.mic.soundPressureLevel(10);
    //double sound_pressure = CircuitPlayground.mic.soundPressureLevel(20);
    //double sound_pressure = CircuitPlayground.mic.soundPressureLevel(40);
    //double sound_pressure = CircuitPlayground.mic.soundPressureLevel(80);

    //// test Red #13 LED
    //if (sound_pressure > 62)
    //{
    //    CircuitPlayground.redLED(HIGH);
    //}
    //else
    //{
    //    CircuitPlayground.redLED(LOW);
    //}
    sensor_info.sound_sensor_info.setData(sound_pressure);
    
}

void setup() {
    //while (!Serial);
    Serial.begin(115200);
    Serial.println("Circuit Playground test!");
    CircuitPlayground.begin();

    gauge_acc.gauge_color_type = Gauge::GaugeColorType::COLOR_SINGLE;
    gauge_acc.rgb = RGB(255,0,0);
    gauge_acc.bindLedPin(9,0);
    gauge_acc.bindLedPin(8,1);
    gauge_acc.bindLedPin(7,2);
    gauge_acc.bindLedPin(6,3);
    gauge_acc.bindLedPin(5,4);
    gauge_acc.bindLedPin(4,5);
    gauge_acc.bindLedPin(3,6);
    gauge_acc.bindLedPin(2,7);
    gauge_acc.bindLedPin(1,8);
    gauge_acc.bindLedPin(0,9);
    
    gauge_light.gauge_color_type = Gauge::GaugeColorType::COLOR_SINGLE;
    gauge_light.rgb = RGB(0,50,0);
    gauge_light.bindLedPin(9,0);
    gauge_light.bindLedPin(8,1);
    gauge_light.bindLedPin(7,2);
    gauge_light.bindLedPin(6,3);
    gauge_light.bindLedPin(5,4);
    gauge_light.bindLedPin(4,5);
    gauge_light.bindLedPin(3,6);
    gauge_light.bindLedPin(2,7);
    gauge_light.bindLedPin(1,8);
    gauge_light.bindLedPin(0,9);
    
    gauge_sound.gauge_color_type = Gauge::GaugeColorType::COLOR_SINGLE;
    gauge_sound.rgb = RGB(0,0,50);
    gauge_sound.bindLedPin(9,0);
    gauge_sound.bindLedPin(8,1);
    gauge_sound.bindLedPin(7,2);
    gauge_sound.bindLedPin(6,3);
    gauge_sound.bindLedPin(5,4);
    gauge_sound.bindLedPin(4,5);
    gauge_sound.bindLedPin(3,6);
    gauge_sound.bindLedPin(2,7);
    gauge_sound.bindLedPin(1,8);
    gauge_sound.bindLedPin(0,9);
    
    gauge_car_motion.gauge_color_type = Gauge::GaugeColorType::COLOR_SINGLE;
    gauge_car_motion.rgb = RGB(55,55,0);
    gauge_car_motion.bindLedPin(9,0);
    gauge_car_motion.bindLedPin(8,1);
    gauge_car_motion.bindLedPin(7,2);
    gauge_car_motion.bindLedPin(6,3);
    gauge_car_motion.bindLedPin(5,4);
    gauge_car_motion.bindLedPin(4,5);
    gauge_car_motion.bindLedPin(3,6);
    gauge_car_motion.bindLedPin(2,7);
    gauge_car_motion.bindLedPin(1,8);
    gauge_car_motion.bindLedPin(0,9);
}


void loop() {
    CircuitPlayground.redLED(true);
    //Serial.print("[");
    //Serial.print(counter);
    //Serial.print("] ");
    counter++;


    updateSensorInfo(sensor_info);

    car_motion.setSensorInfo(sensor_info);

    if(sensor_info.button_sensor_info.right_button_pressed)
    {
        CircuitPlayground.playTone(880,10);
        display_mode++;
        display_mode = display_mode%display_mode_number;
    }
    
    if(sensor_info.button_sensor_info.left_button_pressed)
    {
        CircuitPlayground.playTone(440,10);
        display_mode--;
        if(display_mode<0)
        {
            display_mode = display_mode_number-1;
        }
    }

    // print all sensor infor
    //sensor_info.serial_print();
    sensor_info.serial_print2();
    

    //LED 
    //Serial.print("display_mode: ");
    //Serial.println(display_mode);
    if(display_mode==0)
    {
        //Serial.print("car_motion.car_g_total: ");
        //Serial.println(car_motion.car_g_total);
        gauge_car_motion.displayColorMapping(car_motion.car_g_total);
        //Serial.print("car_motion.car_g_total: ");
        //Serial.println(car_motion.car_g_total);
        //Serial.print("car_motion.motion_alarm: ");
        //Serial.println(car_motion.motion_alarm);
        //Serial.print("car_motion_alarm_triggered: ");
        //Serial.println(car_motion_alarm_triggered);
        if(!car_motion_alarm_triggered && car_motion.motion_alarm)
        {
            CircuitPlayground.playTone(611,50);
            delay(25);
            CircuitPlayground.playTone(1222,50);
            car_motion_alarm_triggered = true;
        }

        if(car_motion_alarm_triggered && !car_motion.motion_alarm)
        {
            car_motion_alarm_triggered = false;
        }
    }
    else if(display_mode==1)
    {
      //setLedGauge(sensor_info.temperature_sensor_info.mvd_temperature.mean, 8, 3);
      setLedGaugeTwoDigits(sensor_info.temperature_sensor_info.mvd_temperature.mean);
    }
    else if(display_mode==2)
    {
      gauge_acc.displayColorMapping(sensor_info.accelerometer_sensor_info.acc);
    }
    else if(display_mode==3)
    {
        gauge_light.displayColorMapping(sensor_info.light_sensor_info.mvd_light.mean);
    }
    //else if(display_mode==4)
    //{
    //gauge_sound.displayColorMapping(sensor_info.sound_sensor_info.mvd_sound.mean);
    //}


    CircuitPlayground.redLED(false);
    delay(100);
}

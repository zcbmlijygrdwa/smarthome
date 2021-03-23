#ifndef SENSOR_INFO_HPP
#define SENSOR_INFO_HPP

struct ButtonSensorInfo
{
bool left_button_pressed = false;
bool right_button_pressed = false;
};

struct AccelerometerSensorInfo
{
    double acc = 0.0;
    double acc_x = 0.0;
    double acc_y = 0.0;
    double acc_z = 0.0;

    double acc_x_bias = 1.0;    //??? TODO : need to comfirm this.
    double acc_y_bias = -0.8;    //??? TODO : need to comfirm this.

    MeanVarData mvd_acc_x;
    MeanVarData mvd_acc_y;
    MeanVarData mvd_acc_z;

    bool accelerometer_state = false;

    AccelerometerSensorInfo()
    {
        mvd_acc_x = MeanVarData(1);
        mvd_acc_y = MeanVarData(1);
        mvd_acc_z = MeanVarData(1);
    }

    void setData(double ax, double ay, double az)
    {
        acc_x = ax - acc_x_bias;
        mvd_acc_x.setData(acc_x);
        acc_y = ay - acc_y_bias;
        mvd_acc_y.setData(acc_y);
        acc_z = az;
        mvd_acc_z.setData(acc_z);

        acc = sqrt(acc_x*acc_x + acc_y*acc_y + acc_z*acc_z);
    }
};

struct TemperatureSensorInfo
{
    double temperature = 0.0;
    MeanVarData mvd_temperature;
    bool temperature_state = false;

    TemperatureSensorInfo()
    {
        mvd_temperature = MeanVarData(50);
    }

    void setData(double data)
    {
        temperature = data;
        mvd_temperature.setData(data);
    }
};

struct LightSensorInfo 
{
    double light = 0.0;
    MeanVarData mvd_light;
    bool light_state = false;

    LightSensorInfo()
    {
        mvd_light = MeanVarData(20);
    }

    void setData(double data)
    {
        light = data;
        mvd_light.setData(data);

        if( mvd_light.mean > 25)
        {
            light_state = true;
        }
        else if(mvd_light.mean < 20)
        {
            light_state = false;
        }
    }
};

struct SoundSensorInfo 
{
    double sound = 0.0;
    MeanVarData mvd_sound;
    bool sound_state = false;

    SoundSensorInfo()
    {
        mvd_sound = MeanVarData(50);
    }

    void setData(double data)
    {
        sound = data;
        mvd_sound.setData(data);
    }

};

struct SensorInfo
{
    ButtonSensorInfo button_sensor_info;
    AccelerometerSensorInfo accelerometer_sensor_info;
    TemperatureSensorInfo temperature_sensor_info;
    LightSensorInfo light_sensor_info;
    SoundSensorInfo sound_sensor_info;

    void serial_print()
    {
        /************* TEST LIGHT SENSOR */
        Serial.print("Light: ");
        Serial.print(light_sensor_info.mvd_light.mean);
        Serial.print("|");
        Serial.print(light_sensor_info.mvd_light.var);
        /************* TEST SOUND SENSOR */
        Serial.print(" Sound: ");
        Serial.print(sound_sensor_info.mvd_sound.mean);
        Serial.print("|");
        Serial.print(sound_sensor_info.mvd_sound.var);

        //  /************* TEST ACCEL */
        Serial.print(" Acc: ");
        Serial.print(accelerometer_sensor_info.acc_x);       
        Serial.print(",");
        Serial.print(accelerometer_sensor_info.acc_y);       
        Serial.print(",");
        Serial.print(accelerometer_sensor_info.acc_z);       

        Serial.print("|");
        Serial.print(accelerometer_sensor_info.acc);       

        //Serial.println(" m/s^2");

        /************* TEST THERMISTOR */
        Serial.print(" Temp: ");
        //Serial.println(temp_temp);
        Serial.print(temperature_sensor_info.mvd_temperature.mean);
        Serial.print("|");
        Serial.println(temperature_sensor_info.mvd_temperature.var);
    }

};



///* BELOW are old method to get sensor
//
//  //  /************* TEST CAPTOUCH */
//  //  Serial.print("Capsense #3: "); Serial.println(CircuitPlayground.readCap(3));
//  //  Serial.print("Capsense #2: "); Serial.println(CircuitPlayground.readCap(2));
//  //  if (! CircuitPlayground.isExpress()) {  // CPX does not have this captouch pin
//  //    Serial.print("Capsense #0: "); Serial.println(CircuitPlayground.readCap(0));
//  //  }
//  //  Serial.print("Capsense #1: "); Serial.println(CircuitPlayground.readCap(1));
//  //  Serial.print("Capsense #12: "); Serial.println(CircuitPlayground.readCap(12));
//  //  Serial.print("Capsense #6: "); Serial.println(CircuitPlayground.readCap(6));
//  //  Serial.print("Capsense #9: "); Serial.println(CircuitPlayground.readCap(9));
//  //  Serial.print("Capsense #10: "); Serial.println(CircuitPlayground.readCap(10));
//  //
//  //  /************* TEST SLIDE SWITCH */
//  //  if (CircuitPlayground.slideSwitch()) {
//  //    Serial.println("Slide to the left");
//  //  } else {
//  //    Serial.println("Slide to the right");
//  //    CircuitPlayground.playTone(500 + pixeln * 500, 100);
//  //  }
//  //
//  //  /************* TEST 10 NEOPIXELS */
//  //  CircuitPlayground.setPixelColor(pixeln++, CircuitPlayground.colorWheel(25 * pixeln));
//  //  if (pixeln == 11) {
//  //    pixeln = 0;
//  //    CircuitPlayground.clearPixels();
//  //  }
//
//  /************* TEST BOTH BUTTONS */
//  if (CircuitPlayground.leftButton()) {
//    Serial.println("Left button pressed!");
//  }
//  if (CircuitPlayground.rightButton()) {
//    Serial.println("Right button pressed!");
//  }
//
//  /************* TEST LIGHT SENSOR */
//  Serial.print("Light sensor: ");
//  Serial.print(mean_var_light->mean);
//  Serial.print(":");
//  Serial.print(mean_var_light->var);
//  /************* TEST SOUND SENSOR */
//  Serial.print(",");
//  Serial.print(mean_var_sound->mean);
//  Serial.print(":");
//  Serial.print(mean_var_sound->var);
//
//  //  /************* TEST ACCEL */
//  //  // Display the results (acceleration is measured in m/s*s)
//  //Serial.print("X: ");
//  Serial.print(",");
//  Serial.print(CircuitPlayground.motionX());
//  //Serial.print(" \tY: ");
//  Serial.print(",");
//  Serial.print(CircuitPlayground.motionY());
//  //Serial.print(" \tZ: ");
//  Serial.print(",");
//  Serial.print(CircuitPlayground.motionZ());
//  //Serial.println(" m/s^2");
//
//  /************* TEST THERMISTOR */
//  Serial.print(",");
//  //Serial.println(temp_temp);
//  Serial.print(mean_var_temp->mean);
//  Serial.print(":");
//  Serial.println(mean_var_temp->var);
//
//*/

#endif

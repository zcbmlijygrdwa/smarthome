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


    double acc_x_scale = 1.011693;
    double acc_y_scale = 1.047120;
    double acc_z_scale = 0.982878;

    double acc_x_bias = -0.178910;
    double acc_y_bias = 0.016701;
    double acc_z_bias = 0.190538;
    

    MeanVarData mvd_acc_x;
    MeanVarData mvd_acc_y;
    MeanVarData mvd_acc_z;

    bool accelerometer_state = false;

    AccelerometerSensorInfo()
    {
        mvd_acc_x = MeanVarData(10);
        mvd_acc_y = MeanVarData(10);
        mvd_acc_z = MeanVarData(10);
    }

    void setData(double ax, double ay, double az)
    {
        acc_x = ax*acc_x_scale+acc_x_bias;
        mvd_acc_x.setData(acc_x);
        acc_y = ay*acc_y_scale+acc_y_bias;
        mvd_acc_y.setData(acc_y);
        acc_z = az*acc_z_scale+acc_z_bias;
        mvd_acc_z.setData(acc_z);

        acc = sqrt(acc_x*acc_x + acc_y*acc_y + acc_z*acc_z) - 9.8;
    }
};

struct TemperatureSensorInfo
{
    enum Unit {Celsius, Fahrenheit};
    double temperature = 0.0;
    MeanVarData mvd_temperature;
    bool temperature_state = false;

    Unit temperature_unit = Unit::Celsius;

    TemperatureSensorInfo()
    {
        mvd_temperature = MeanVarData(100);
    }

    void setTemperatureUnit(Unit unit)
    {
        temperature_unit = unit;
    }

    void setData(double data)
    {
        if(temperature_unit == Unit::Fahrenheit)
            temperature = data*(9.0/5.0) + 32.0;
        else
            temperature = data;

        mvd_temperature.setData(temperature);
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

struct SerialMessage
{

    int start_flag1 = 43; // 00000000 00000000 00000000 00101011
    int start_flag2 = 0;  // 00000000 00000000 00000000 00000000
    
  
    // light
    double light_mean = 0;
    double light_var = 0;

    // sound
    double sound_mean = 0;
    double sound_var = 0;

    // temperature
    double temperature_mean = 0;
    double temperature_var = 0;

    // acceleration
    double a_x = 0;
    double a_y = 0;
    double a_z = 0;
  
};

struct SensorInfo
{
    ButtonSensorInfo button_sensor_info;
    AccelerometerSensorInfo accelerometer_sensor_info;
    TemperatureSensorInfo temperature_sensor_info;
    LightSensorInfo light_sensor_info;
    SoundSensorInfo sound_sensor_info;

    SensorInfo()
    {
        //temperature_sensor_info.setTemperatureUnit(TemperatureSensorInfo::Unit::Fahrenheit);
    }

    void serial_print2()
    {
      SerialMessage* sm = new SerialMessage();

      sm->light_mean = light_sensor_info.mvd_light.mean;
      sm->light_var = light_sensor_info.mvd_light.var;

      sm->sound_mean = sound_sensor_info.mvd_sound.mean;
      sm->sound_var = sound_sensor_info.mvd_sound.var;

      sm->temperature_mean = temperature_sensor_info.mvd_temperature.mean;
      sm->temperature_var = temperature_sensor_info.mvd_temperature.var;

      sm->a_x = accelerometer_sensor_info.acc_x;
      sm->a_y = accelerometer_sensor_info.acc_y;
      sm->a_z = accelerometer_sensor_info.acc_z;
      
      Serial.write((char*)sm,sizeof(SerialMessage));
      delete sm;
    }

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

#ifndef SERIAL_OUTPUTER_HPP
#define SERIAL_OUTPUTER_HPP

struct SerialOutputer
{

    SerialOutputer()
    {

    }

    ~SerialOutputer()
    {

    }

    static void print(SensorInfo sensor_info)
    {

        /************* TEST BOTH BUTTONS */
        if (CircuitPlayground.leftButton()) {
            Serial.println("Left button pressed!");
        }
        if (CircuitPlayground.rightButton()) {
            Serial.println("Right button pressed!");
        }

        /************* TEST LIGHT SENSOR */
        Serial.print("Light sensor: ");
        Serial.print(mean_var_light->mean);
        Serial.print(":");
        Serial.print(mean_var_light->var);
        /************* TEST SOUND SENSOR */
        Serial.print(",");
        Serial.print(mean_var_sound->mean);
        Serial.print(":");
        Serial.print(mean_var_sound->var);

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
        //Serial.println(temp_temp);
        Serial.print(mean_var_temp->mean);
        Serial.print(":");
        Serial.println(mean_var_temp->var);

    }

};

#endif

#ifndef car_motion_hpp
#define car_motion_hpp


#include "common_types.h"


struct CarMotion 
{

    double car_g_total = 0;
    double car_g_alarm_threshold = 10;
    bool motion_alarm = false;

    CarMotion()
    {
    }

    ~CarMotion()
    {
    }

    void setSensorInfo(const SensorInfo& sensor_info)
    {        
        double acc_in = sensor_info.accelerometer_sensor_info.acc;
        //double acc_in = sqrt(acc_x*acc_x  + acc_z*acc_z);

        if(car_g_total < acc_in)
        {
            //impulse
            car_g_total = acc_in;
        }
        else
        {
            //decay
            car_g_total += 0.01*(acc_in - car_g_total);
        }

        if(car_g_total > car_g_alarm_threshold)
        {
            motion_alarm = true;
        }
        else
        {
            motion_alarm = false;
        }
    }


};

#endif

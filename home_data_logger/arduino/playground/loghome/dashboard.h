#ifndef my_color_hpp
#define my_color_hpp

#define RGB_LED_NUMBER 10;

struct Dashboard
{

    enum DashboardGaugeType
    {
        GAUGE_SINGLE_CW = 0,  //single gauge, clock-wise
        GAUGE_SINGLE_CCW = 1, //single gauge, counter clock-wise
        GAUGE_DOUBLE = 2      //double gauges
    }

};

#endif

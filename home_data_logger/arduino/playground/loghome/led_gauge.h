#ifndef led_gauge_hpp
#define led_gauge_hpp

#include "my_color.h"

//#include "/home/zhenyu/kf_demo/test.hpp"

void setLedGauge(double value, double min_val, double increamental)
{
    int led_num = 10;

    if (value > min_val)
    {
        double delta_val = value - min_val;
        int led_lit_count = delta_val/increamental;
        for(int pixeln = 0; pixeln < led_num; pixeln++)
        {
            if(pixeln <= led_lit_count)
            {
                //CircuitPlayground.setPixelColor(pixeln, CircuitPlayground.colorWheel(25 * pixeln));
                RGB rgb = my_colorWheel(25 * pixeln);
                CircuitPlayground.setPixelColor(pixeln, rgb.r, rgb.g, rgb.b);
                //std::cout<<"led["<<pixeln<<"] on "<<std::endl;
            }
            else
            {
                CircuitPlayground.setPixelColor(pixeln, 0, 0, 0);
                //std::cout<<"led["<<pixeln<<"] off "<<std::endl;
            }
        }
    }
    else
    {
        CircuitPlayground.clearPixels();
    }
}

void setLedGaugeTwoDigits(double value)
{
    int led_num = 10;

    int value_int = value;
    int tens = value_int/10;
    int digits = value_int%10;

    int greens = digits/2;
    int blues = digits-greens*2;

    tens = min(tens, 5);

    //Serial.print("value_int = ");
    //Serial.print(value_int);
    //Serial.print(", tens = ");
    //Serial.print(tens);
    //Serial.print(", digits = ");
    //Serial.print(digits);
    //Serial.print(", greens = ");
    //Serial.print(greens);
    //Serial.print(", blues = ");
    //Serial.println(blues);

    for(int i = 5 ; i < 10 ; i++)
    {
        if(10-i<=tens)
            CircuitPlayground.setPixelColor(i, 255, 0 ,0);
        else
            CircuitPlayground.setPixelColor(i, 0, 0 ,0);
    }
    
    //draw blue
    for(int i = 0 ; i < 5 ; i++)
    {
        if(i<greens)
                CircuitPlayground.setPixelColor(i, 0, 255 ,0);
        else if(blues > 0 && i<greens+blues)
                CircuitPlayground.setPixelColor(i, 0, 0 ,255);
        else
            CircuitPlayground.setPixelColor(i, 0, 0 ,0);
    }

}

#endif

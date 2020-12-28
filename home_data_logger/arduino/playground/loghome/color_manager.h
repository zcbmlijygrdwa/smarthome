#ifndef color_manager_hpp
#define color_manager_hpp

#include "my_color.h"

struct ColorManager 
{
    enum ColorThemeType
    {
        COLOR_THEME_MAPPING = 0, //multi-color, each led has different color, color change with led index
        COLOR_THEME_CHANGING = 1 //multi-color, all led have the same color but color change with value
    };

    RGB rgb;
    double value_min = 0;
    double value_max = 0;

    RGB getColor(double value)
    {
        uint8_t WheelPos = 255*( (value-value_min) / (value_max - value_min) );
        //For R
        if(WheelPos<=127)
        {
            rgb.r = 0;
        }
        else
        {
            rgb.r = (WheelPos-127)*2-1;
        }

        //For G
        if(WheelPos<128)
        {
            rgb.g = (WheelPos*2)*0.3;
        }
        else
        {
            rgb.g = (256 - (WheelPos-127)*2)*0.3;
        }

        //For B
        if(WheelPos>=128)
        {
            rgb.b = 0;
        }
        else
        {
            rgb.b = 255 - 2*WheelPos;
        }

        //rgb.r = WheelPos;
        //rgb.g = 0;
        //rgb.b = 255 - WheelPos;

        return rgb;
    }
};

#endif

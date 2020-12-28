#ifndef gauge_hpp
#define gauge_hpp

#include "color_manager.h"

struct Gauge 
{
    enum GaugeColorType
    {
        COLOR_SINGLE = 0,   //single color
        COLOR_MAPPING = 1, //multi-color, each led has different color, color change with led index
        COLOR_CHANGING = 2 //multi-color, all led have the same color but color change with value
    };

    int* led_pin_map = NULL;
    int  led_number = -1;
    double value_min = 0;
    double value_max = 0;

    GaugeColorType gauge_color_type = COLOR_SINGLE;
    RGB rgb;

    ColorManager color_manager;

    Gauge(int led_n, double v_min, double v_max)
    {
        value_min = v_min;
        value_max = v_max;
        led_number = led_n;
        led_pin_map = (int*)calloc(led_number, sizeof(int));

        //set default single rgb
        rgb.r = 255;
        rgb.g = 0;
        rgb.b = 0;
    }

    ~Gauge()
    {
        free(led_pin_map);
        printf("led_pin_map of size[%d] freed\n", led_number);
    }

    void bindLedPin(int pin, int index)
    {
        led_pin_map[index] = pin;
    }

    void displayColorMapping(double value)
    {
        if (value > value_min)
        {
            double delta_val = value - value_min;
            int led_lit_count = led_number*(delta_val/(value_max-value_min));
            for(int pixeln = 0; pixeln < led_number; pixeln++)
            {
                if(pixeln < led_lit_count)
                {
                    //CircuitPlayground.setPixelColor(pixeln, CircuitPlayground.colorWheel(25 * pixeln));
                    if(gauge_color_type == COLOR_MAPPING)
                    {
                        color_manager.value_min = pixeln;
                        color_manager.value_max = led_number;
                        rgb = color_manager.getColor(pixeln);
                    }
                    else if(gauge_color_type == COLOR_CHANGING)
                    {
                        color_manager.value_min = value_min;
                        color_manager.value_max = value_max;
                        rgb = color_manager.getColor(value);
                    }
                    CircuitPlayground.setPixelColor(led_pin_map[pixeln], rgb.r, rgb.g, rgb.b);
                    //std::cout<<"led["<<pixeln<<"] on "<<std::endl;
                }
                else
                {
                    CircuitPlayground.setPixelColor(led_pin_map[pixeln], 0, 0, 0);
                    //std::cout<<"led["<<pixeln<<"] off "<<std::endl;
                }
            }
        }
        else
        {
            CircuitPlayground.clearPixels();
        }
    }
    void display(double value)
    {
    }


};

#endif

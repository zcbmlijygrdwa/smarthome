#ifndef my_color_hpp
#define my_color_hpp

struct RGB
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
};

RGB my_colorWheel(uint8_t WheelPos)
{
    RGB rgb;

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

#endif

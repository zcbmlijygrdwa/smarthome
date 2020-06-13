#ifndef led_gauge_hpp
#define led_gauge_hpp

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
                CircuitPlayground.setPixelColor(pixeln, CircuitPlayground.colorWheel(25 * pixeln));
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
        //CircuitPlayground.clearPixels();
    }
}

#endif

#include <iostream>

#include "../led_gauge.h"

#include "/home/zhenyu/kf_demo/test.hpp"

int main(int argc, char** argv)
{
    int test_val = 10;
    int increamental = 3;
    
    test_val = 1;
    printv(test_val);
    setLedGauge(test_val, 0 , increamental);

    test_val = 2;
    printv(test_val);
    setLedGauge(test_val, 0 , increamental);
    
    test_val = 3;
    printv(test_val);
    setLedGauge(test_val, 0 , increamental);
    
    test_val = 4;
    printv(test_val);
    setLedGauge(test_val, 0 , increamental);

    test_val = 17;
    printv(test_val);
    setLedGauge(test_val, 0 , increamental);

    test_val = 27;
    printv(test_val);
    setLedGauge(test_val, 0 , increamental);

    test_val = 37;
    printv(test_val);
    setLedGauge(test_val, 0 , increamental);
    return 0;
}

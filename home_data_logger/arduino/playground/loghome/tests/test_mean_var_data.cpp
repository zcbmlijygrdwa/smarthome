#include <iostream>

#include "../mean_var_data.h"

#include "/home/zhenyu/kf_demo/test.hpp"

int main(int argc, char** argv)
{
    MeanVarData* mean_var_temp = new MeanVarData(10);
    for(int i = 1 ; i <= 5 ; i++)
    {
    double temp_temp = i;
    printv("-----------");
    printv(temp_temp);
    calculateMeanAndVar(temp_temp, mean_var_temp);
    printv(mean_var_temp->mean);
    printv(mean_var_temp->var);
    }
    delete(mean_var_temp);

    return 0;
}

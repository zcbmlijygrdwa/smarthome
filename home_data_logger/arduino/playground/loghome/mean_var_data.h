#ifndef MeanVarData_HPP
#define MeanVarData_HPP

//#include "/home/zhenyu/kf_demo/test.hpp"
#include <math.h>

struct MeanVarData
{
    int window_size = 50;
    int pointer = 0;
    double* data_buffer;
    double mean = 0;
    double var = 0;
    bool is_full = false;

    MeanVarData()
    {
        data_buffer = (double*)calloc(window_size, sizeof(double));
    }

    MeanVarData(int new_window_size)
    {
        window_size = new_window_size;
        data_buffer = (double*)calloc(window_size, sizeof(double));
    }

    ~MeanVarData()
    {
        //printv(data_buffer);
        free(data_buffer);
    }
};

void calculateMeanAndVar(double new_data, MeanVarData* mean_var_data)
{
    mean_var_data->data_buffer[mean_var_data->pointer] = new_data;

    double sum = 0;

    if (mean_var_data->pointer < mean_var_data->window_size)
    {
        mean_var_data->pointer++;
    }
    else
    {
        mean_var_data->is_full = true;
        mean_var_data->pointer = 0;
    }

    int data_size = 0;
    if (mean_var_data->is_full)
    {
        data_size = mean_var_data->window_size;
    }
    else
    {
        data_size = mean_var_data->pointer;
    }
    for (int i = 0 ; i < data_size ; ++i)
    {
        sum += mean_var_data->data_buffer[i];
    }
    mean_var_data->mean = sum / data_size;

    mean_var_data->var = 0;
    for (int i = 0 ; i < data_size ; ++i)
    {
        double temp = (mean_var_data->data_buffer[i] - mean_var_data->mean);
        mean_var_data->var += temp * temp;
    }
    mean_var_data->var = mean_var_data->var/data_size;
    //double standard_deviation = sqrt(mean_var_data->var);
}

#endif

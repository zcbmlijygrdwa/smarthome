/***
 * This example expects the serial port has a loopback on it.
 *
 * Alternatively, you could use an Arduino:
 *
 * <pre>
 *  void setup() {
 *    Serial.begin(<insert your baudrate here>);
 *  }
 *
 *  void loop() {
 *    if (Serial.available()) {
 *      Serial.write(Serial.read());
 *    }
 *  }
 * </pre>
 */

#include <string>
#include <chrono>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <signal.h>
#include <chrono>

// OS Specific sleep
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "serial/serial.h"
#include "../../../../smt/OAPI-Bot/linux_curl/cpp_program/include/test.hpp"

#include "sensor_reader.hpp"
#include "trailing_smoother.hpp"

using std::string;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

std::ofstream myfile;

void signal_callback_handler(int signum) {
    cout << "Caught signal " << signum << endl;
    // Terminate program
    myfile.close();
    exit(signum);
}

void my_sleep(unsigned long long milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds); // 100 ms
#else
    usleep(milliseconds*1000); // 100 ms
#endif
}

void print_usage()
{
    cerr << "Usage: test_serial {-e|<serial port address>} ";
    cerr << "<baudrate> [test string]" << endl;
}

int run(int argc, char **argv)
{
    if(argc < 2) {
        print_usage();
        return 0;
    }

    // Argument 1 is the serial port or enumerate flag
    string port(argv[1]);

    if( port == "-e" ) {
        //enumerate_ports();
        return 0;
    }
    else if( argc < 3 ) {
        print_usage();
        return 1;
    }

    // Argument 2 is the baudrate
    unsigned long long baud = 0;
#if defined(WIN32) && !defined(__MINGW32__)
    sscanf_s(argv[2], "%lu", &baud);
#else
    sscanf(argv[2], "%lu", &baud);
#endif

    // port, baudrate, timeout in milliseconds
    serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(10));

    cout << "Is the serial port open?";
    if(my_serial.isOpen())
        cout << " Yes." << endl;
    else
        cout << " No." << endl;

    // Get the Test string
    int count = 0;
    string test_string;
    if (argc == 4) {
        test_string = argv[3];
    } else {
        test_string = "Testing.";
    }

    // Test the timeout, there should be 1 second between prints
    cout << "Timeout == 1000ms, asking for 1 more byte than written." << endl;
    myfile.open ("arduino_data.txt");
    std::string buffer;
    std::string temp_data_str;
    while (true) {

        string result = my_serial.read(1000);

        buffer = buffer + result;

        //cout << "Iteration: " << count << ", Bytes written: ";
        //cout << result.length() << ", String read: " << result << endl;
        int first_occur = buffer.find('\n');
        if(first_occur > 0)
        {
            // if find \n, take the data from buffer and parse it
            temp_data_str = buffer.substr (0,first_occur);
            //printv(temp_data_str);
            long long microsecondsUTC = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            //printv(microsecondsUTC);
            SensorReader::DataFormat df(temp_data_str,microsecondsUTC);
            printv(df.str());
            if(df.is_data_good)
            {
                //printv(df.strLite());
                myfile << df.strLite();
                myfile << "\n";
            }

            buffer = buffer.substr(first_occur+1);
            //printv(buffer);
        }
        //printv(first_occur);

        count += 1;
    }
    myfile.close();
    return 0;
}

int main(int argc, char **argv) {
    //// Register signal and signal handler
    //signal(SIGINT, signal_callback_handler);
    //try {
    //    return run(argc, argv);
    //} catch (exception &e) {
    //    cerr << "Unhandled Exception: " << e.what() << endl;
    //}
    if( argc != 3)
    {
        print_usage();
        return 1;
    }

    unsigned long long baud = 0;
#if defined(WIN32) && !defined(__MINGW32__)
    sscanf_s(argv[2], "%lu", &baud);
#else
    sscanf(argv[2], "%lu", &baud);
#endif

    SensorReader sr(argv[1], baud);
    printv("before start");
    sr.startReading();

    TrailingSmoother ts_temp(0.01);
    TrailingSmoother ts_sound(0.01);
    TrailingSmoother ts_light(0.01);
    std::chrono::steady_clock::time_point quiet_start;
    bool quiet_start_int = false;
    int light_status = 1;

    while(true)
    {
        SensorReader::DataFormat df = sr.fetchData();
        if(df.is_data_good)
        {
            printv(df.str());


            double reading_temp = ts_temp.add(df.temperature);
            double reading_sound = ts_sound.add(df.soundPressure);
            double reading_light = ts_light.add(df.light);
            printv(reading_temp);
            printv(reading_sound);
            printv(reading_light);

            if(reading_sound<57.5)
            {
                if(quiet_start_int)
                {
                    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); 
                    long long int time_diff = std::chrono::duration_cast<std::chrono::microseconds>(end - quiet_start).count()/1000000;
                    std::cout << "Has been quiet for " << time_diff<<" seconds, light_status = "<<light_status<< std::endl;
                    if(time_diff>30 && time_diff< 60)
                    {
                        if(light_status==1)
                        {
                            system("tplight hsb 192.168.0.129 190 1 50 -t 5000 &");
                            light_status = 2;
                        }
                    }
                    else if(time_diff>=60 && time_diff< 120)
                    {
                        if(light_status==2)
                        {
                            system("tplight hsb 192.168.0.129 190 1 20 -t 5000 &");
                            light_status = 3;
                        }
                    }
                    else if(time_diff>=120 && time_diff< 180)
                    {
                        if(light_status==3)
                        {
                            system("tplight hsb 192.168.0.129 190 1 10 -t 5000 &");
                            light_status = 4;
                        }
                    }
                    else if(time_diff>=180)
                    {
                        if(light_status==4)
                        {
                            system("tplight off 192.168.0.129 -t 5000 &");
                            light_status = 0;
                        }
                    }
                }
                else
                {
                    quiet_start = std::chrono::steady_clock::now();
                    quiet_start_int = true;
                }
            }
            else
            {
                quiet_start = std::chrono::steady_clock::now();
                if(light_status!=1)
                {
                    system("tplight on 192.168.0.129 -t 5000 &");
                    light_status = 1;
                }
            }

        }
        my_sleep(10);
    }

    return 0;
}

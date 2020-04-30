#include <iostream>
#include <chrono>
#include <fstream>

#include <sys/socket.h>

#include <netinet/in.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../smt/OAPI-Bot/linux_curl/cpp_program/include/test.hpp"

#include "serial/serial.h"

std::ofstream myfile;

struct DataFormat
{
    bool is_data_good = false;
    long long unix_time = -1;
    double light = -1;
    double soundPressure = -1;
    double motionX = -1;
    double motionY = -1;
    double motionZ = -1;
    double temperature = -1;

    DataFormat(std::string data_str, long long unix_time_in)
    {
        printv(unix_time_in);
        size_t pos = 0;
        std::string token;
        std::string delimiter = ",";
        std::vector<double> data_vec;

        while ((pos = data_str.find(delimiter)) != std::string::npos)
        {
            token = data_str.substr(0, pos);
            //std::cout << token << std::endl;
            data_vec.push_back(std::stod(token));
            data_str.erase(0, pos + delimiter.length());
        }
        data_vec.push_back(std::stod(data_str));

        //printv(data_vec.size());
        if(data_vec.size() == 6)
        {
            unix_time = unix_time_in;
            is_data_good = true;
            light         = data_vec[0];
            soundPressure = data_vec[1];
            motionX       = data_vec[2];
            motionY       = data_vec[3];
            motionZ       = data_vec[4];
            temperature   = data_vec[5];
        }
    }

    std::string str()
    {
        std::stringstream ss;
        if(is_data_good)
        {
            ss<<"unix_time: "<<unix_time<<", light: "<<light<<", soundPressure: "<<soundPressure<<", acc: {"<<motionX<<","<<motionY<<","<<motionZ<<"}, temperature: "<<temperature;
        }
        else
        {
            ss<<"BadData...";
        }
        return ss.str();
    }

    std::string strLite()
    {
        std::stringstream ss;
        ss<<unix_time<<","<<light<<","<<soundPressure<<","<<motionX<<","<<motionY<<","<<motionZ<<","<<temperature;
        return ss.str();
    }
};


void enumerate_ports()
{
    std::vector<serial::PortInfo> devices_found = serial::list_ports();

    std::vector<serial::PortInfo>::iterator iter = devices_found.begin();

    while( iter != devices_found.end() )
    {
        serial::PortInfo device = *iter++;

        printf( "(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
                device.hardware_id.c_str() );
    }
}

void print_usage()
{
    std::cerr << "Usage: test_serial {-e|<serial port address>} ";
    std::cerr << "<baudrate> [test string]" << std::endl;
}

#define PORT 8080 
int main(int argc, char** argv)
{

    //test send data through TCP/IP
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );


    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }


    while(true)
    {
        char buffer[1024] = {0};
        valread = read( new_socket , buffer, 1024);
        printf("%s\n",buffer );
        send(new_socket , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");
    }
    return 0;


    //if(argc < 2) {
    //    print_usage();
    //    return 0;
    //}

    //// Argument 1 is the serial port or enumerate flag
    //std::string port(argv[1]);

    //if( port == "-e" ) {
    //    enumerate_ports();
    //    return 0;
    //}
    //else if( argc < 3 ) {
    //    print_usage();
    //    return 1;
    //}
    //// Argument 2 is the baudrate
    //unsigned long long baud = 0;
    //sscanf(argv[2], "%lu", &baud);

    //// port, baudrate, timeout in milliseconds
    //serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(10));

    //std::cout << "Is the serial port open?";
    //if(my_serial.isOpen())
    //    std::cout << " Yes." << std::endl;
    //else
    //    std::cout << " No." << std::endl;

    //// Get the Test string
    //int count = 0;
    //std::string test_string;
    //if (argc == 4) {
    //    test_string = argv[3];
    //} else {
    //    test_string = "Testing.";
    //}
    //// Test the timeout, there should be 1 second between prints
    //std::cout << "Timeout == 1000ms, asking for 1 more byte than written." << std::endl;
    //myfile.open ("arduino_data.txt");
    //std::string buffer;
    //std::string temp_data_str;
    //while (true) {

    //    std::string result = my_serial.read(1000);

    //    buffer = buffer + result;

    //    //cout << "Iteration: " << count << ", Bytes written: ";
    //    //cout << result.length() << ", String read: " << result << endl;
    //    int first_occur = buffer.find('\n');
    //    if(first_occur > 0)
    //    {
    //        // if find \n, take the data from buffer and parse it
    //        temp_data_str = buffer.substr (0,first_occur);
    //        //printv(temp_data_str);
    //        long long microsecondsUTC = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    //        //printv(microsecondsUTC);
    //        DataFormat df(temp_data_str,microsecondsUTC);
    //        printv(df.str());
    //        if(df.is_data_good)
    //        {
    //            //printv(df.strLite());
    //            myfile << df.strLite();
    //            myfile << "\n";
    //        }

    //        buffer = buffer.substr(first_occur+1);
    //        //printv(buffer);
    //    }
    //    //printv(first_occur);

    //    count += 1;
    //}
    //myfile.close();

    return 0;
}

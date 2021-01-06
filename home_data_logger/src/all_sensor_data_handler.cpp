////to compile with g++: g++ -o all_sensor_data_handler all_sensor_data_handler.cpp -lpthread && ./all_sensor_data_handler

// receiver 
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <thread>
#include <fstream>
#include <vector>
#include <iomanip>

//#include "/home/zhenyu/pointcloud-georeference/valk_lidar/offline_common/valk_lidar_shared/include/shared/test.hpp"

using namespace std;

struct SensorState
{
    double temperature = 0;
    double sound = 0;
    bool isSubscribing = false;
};

void startSubscribing(SensorState* sensor_state)
{
    sensor_state->isSubscribing = true;
}

std::vector<std::string> splitString(std::string str, std::string delimiter)
{
    //reference: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
    std::vector<std::string> tokens;
    //printv(str);
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        //std::cout << token << std::endl;
        tokens.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    //printv(str.size());
    if(str.size()>0)
    {
        tokens.push_back(str);
    }
    return tokens;
}

void parseSensorData(int port, std::string data_str, SensorState* sensor_state)
{
    if(port == 5000)
    {
        std::vector<std::string> tokens = splitString(data_str, ","); 
        ///printvec(tokens);
        sensor_state->temperature = std::stod(tokens[10]);
    }
    else if(port == 6000)
    {
        std::vector<std::string> tokens = splitString(data_str, ":"); 
        //printvec(tokens);
        std::vector<std::string> tokens_sound = splitString(tokens[2], "|"); 
        sensor_state->sound = std::stod(tokens_sound[0]);
        std::vector<std::string> tokens_temp = splitString(tokens[4], "|"); 
        //printvec(tokens_temp);
        sensor_state->temperature = std::stod(tokens_temp[0]);
    }
    else
    {
        //return -1;
    }
}

void bindSensorPort(int port, SensorState* sensor_state)
{
    std::cout<<"Binding sensor with port: "<<port<<std::endl;
    //std::cout<<"sensor_state = "<<sensor_state<<std::endl;
    setvbuf(stdout, NULL, _IONBF, 0); 
    fflush(stdout); 

    // binding address
    struct sockaddr_in addrto;
    bzero(&addrto, sizeof(struct sockaddr_in));
    addrto.sin_family = AF_INET;
    addrto.sin_addr.s_addr = htonl(INADDR_ANY);
    addrto.sin_port = htons(port);

    // broadcasting address
    struct sockaddr_in from;
    bzero(&from, sizeof(struct sockaddr_in));
    from.sin_family = AF_INET;
    from.sin_addr.s_addr = htonl(INADDR_ANY);
    from.sin_port = htons(port);

    int sock = -1;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {   
        cout<<"socket error"<<endl;	
        //return false;
        return ;
    }   

    const int opt = 1;
    //setting type to broadcast
    int nb = 0;
    nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
    if(nb == -1)
    {
        cout<<"set socket error..."<<endl;
        //return false;
        return ;
    }

    if(bind(sock,(struct sockaddr *)&(addrto), sizeof(struct sockaddr_in)) == -1) 
    {   
        cout<<"bind error..."<<endl;
        //return false;
        return ;
    }

    int len = sizeof(sockaddr_in);
    char smsg[100] = {0};

    while(1)
    {
        if(!sensor_state->isSubscribing)
        {
            std::cout<<"port "<<port<<" waiting..."<<std::endl;
            sleep(1.0);
            continue;
        }
        //receive message from broadcast address
        int ret=recvfrom(sock, smsg, 100, 0, (struct sockaddr*)&from,(socklen_t*)&len);
        if(ret<=0)
        {
            cout<<"read error...."<<sock<<endl;
        }
        else
        {		
            //printf("%s\n", smsg);	
            std::string data_str(smsg);
            //std::cout<<"data_str: "<<data_str<<std::endl;
            parseSensorData(port, data_str, sensor_state);
            //printv(sensor_state->temperature);
        }

        //sleep(0.005);
    }
}

int main()
{
    std::cout<<"main"<<std::endl;

    SensorState* sensor_state_arduino = (SensorState*) calloc(1,sizeof(SensorState));
    SensorState* sensor_state_phone = (SensorState*) calloc(1,sizeof(SensorState));

    std::thread sensor_thread_arduino = std::thread(&bindSensorPort, 6000, sensor_state_arduino);
    if(sensor_thread_arduino.joinable())
    {
        //sensor_thread_arduino.join();
        sensor_thread_arduino.detach();
    }

    std::thread sensor_thread_phone = std::thread(&bindSensorPort, 5000, sensor_state_phone);
    if(sensor_thread_phone.joinable())
    {
        //sensor_thread_phone.join();
        sensor_thread_phone.detach();
    }

    startSubscribing(sensor_state_arduino);
    startSubscribing(sensor_state_phone);

    std::ofstream myfile;
    std::string data_file_name = "sensor_data.txt";
    myfile.open (data_file_name); 
    myfile.close();

    while(1)
    {
        long long microsecondsUTC = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        double secondsUTC = microsecondsUTC/1000000;
        std::cout<<"\n====== UTC "<<std::setprecision(16)<<secondsUTC<<" ======"<<std::endl;
        std::cout<<"sensor_state_arduino->temperature = "<<sensor_state_arduino->temperature<<std::endl;
        std::cout<<"sensor_state_arduino->sound = "<<sensor_state_arduino->sound<<std::endl;
        std::cout<<"sensor_state_phone->temperature = "<<sensor_state_phone->temperature<<std::endl;
        myfile.open (data_file_name, std::ios::app);
        //myfile << "111\n";
        myfile << std::setprecision(16)<<secondsUTC<<","<<sensor_state_arduino->temperature <<","<< sensor_state_phone->temperature<<","<<sensor_state_arduino->sound<<"\n";
        myfile.close();
        sleep(10);
    }
    return 0;
}

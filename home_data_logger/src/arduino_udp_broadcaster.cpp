//to compile with g++: g++ -o arduino_udp_broadcaster arduino_udp_broadcaster.cpp  -I../serial/include/ ../serial/build/libserial.a


//sender
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include <string>
#include <chrono>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <signal.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "serial/serial.h"

using namespace std;

int main()
{
    //setup UDP

    //send over UDP
	setvbuf(stdout, NULL, _IONBF, 0); 
	fflush(stdout); 
 
	int sock = -1;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
	{   
		cout<<"socket error"<<endl;	
		return false;
	}   
	
	const int opt = 1;
    // set type to broadcast
	int nb = 0;
	nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt, sizeof(opt));
	if(nb == -1)
	{
		cout<<"set socket error..."<<endl;
		return false;
	}
 
	struct sockaddr_in addrto;
	bzero(&addrto, sizeof(struct sockaddr_in));
	addrto.sin_family=AF_INET;
	addrto.sin_addr.s_addr=htonl(INADDR_BROADCAST);
	addrto.sin_port=htons(6000);
	int nlen=sizeof(addrto);


    //read serial data
    unsigned long long baud = 9600;
    std::string port = "/dev/ttyACM0";
    serial::Serial serial(port, baud, serial::Timeout::simpleTimeout(10));

    std::string str = "";
    while(true )
    {
        size_t read_size = serial.readline(str);
        if(read_size>0)
        {
            //std::cout<<"str = "<<str<<std::endl;
            //send message from broadcast address
            const char* smsg = str.c_str();
            int ret=sendto(sock, smsg, strlen(smsg), 0, (sockaddr*)&addrto, nlen);
            if(ret<0)
            {
                long long microsecondsUTC = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                double secondsUTC = microsecondsUTC/1000000;
                cout<<"send error...."<<ret<<" @ UTC: "<<secondsUTC<<endl;
            }
            str.clear();
        }
        //std::cout << "read_size = "<<read_size<<std::endl;
    }

    std::cout << "\n\n";
    system("PAUSE");

	return 0;
}

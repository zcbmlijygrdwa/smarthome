#include <stdio.h> // standard input and output library
#include <stdlib.h> // this includes functions regarding memory allocation
#include <string.h> // contains string functions
#include <errno.h> //It defines macros for reporting and retrieving error conditions through error codes
#include <time.h> //contains various functions for manipulating date and time
#include <unistd.h> //contains various constants
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> // defines in_addr structure
#include <sys/socket.h> // for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses
#include "../execute/exec.h"

int main()
{
    time_t clock;
    char dataSending[1024]; // Actually this is called packet in Network Communication, which contain data and send through.
    char dataReceived[1024];
    int clintListn = 0, clintConnt = 0;
    int readLen = 0;
    struct sockaddr_in ipOfServer;
    clintListn = socket(AF_INET, SOCK_STREAM, 0); // creating socket
    memset(&ipOfServer, '0', sizeof(ipOfServer));
    memset(dataSending, '0', sizeof(dataSending));
    ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_addr.s_addr = htonl(INADDR_ANY);
    ipOfServer.sin_port = htons(2017); 		// this is the port number of running server
    bind(clintListn, (struct sockaddr*)&ipOfServer , sizeof(ipOfServer));
    listen(clintListn , 20);

    while(1)
    {
        printf("\n\nHi,Iam running server.Some Client hit me\n"); // whenever a request from client came. It will be processed here.
        clintConnt = accept(clintListn, (struct sockaddr*)NULL, NULL);

        readLen = recv(clintConnt, dataReceived, sizeof(dataReceived), 0);
        if(readLen >0)
        {
           printf("data length from client = %d\n",readLen);
           printf("data from client: \n");
           dataReceived[readLen] = 0;
           printf("%s\n",dataReceived);

        if(readLen>5)
        {
            std::string result = exec("tplink-smarthome-api setPowerState 192.168.0.22:9999 \"true\"");
        }
        else
        {
            std::string result = exec("tplink-smarthome-api setPowerState 192.168.0.22:9999 \"false\"");
        }

        }

        /*
           if((readLen = read(clintConnt, dataReceived, sizeof(dataReceived))) > 0)
           {
           printf("data length from client = %d\n",readLen);
           printf("data from client: \n");
           dataReceived[readLen] = 0;
           printf("%s\n",dataReceived);

        //if(fputs(dataReceived, stdout) == EOF)
        //{
        //    printf("\nStandard output error");
        //}   

        printf("\n");
        }   
         */

        printf("now generating time string\n");

        clock = time(NULL);
        snprintf(dataSending, sizeof(dataSending), "%.24s\r\n", ctime(&clock)); // Printing successful message
        write(clintConnt, dataSending, strlen(dataSending));

        close(clintConnt);
        sleep(1);
    }

    return 0;
}

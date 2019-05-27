#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
 
int main(int argc, char** argv)
{
    int CreateSocket = 0;
    int readLen = 0;
    char dataSending[1024];
    char dataReceived[1024];
    struct sockaddr_in ipOfServer;
 
    memset(dataReceived, '0' ,sizeof(dataReceived));
    snprintf(dataSending, sizeof(dataSending),"helloFromCClient!");
    if((CreateSocket = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("Socket not created \n");
        return 1;
    }
 
    ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_port = htons(2017);
    ipOfServer.sin_addr.s_addr = inet_addr("127.0.0.1");
 
    if(connect(CreateSocket, (struct sockaddr *)&ipOfServer, sizeof(ipOfServer))<0)
    {
        printf("Connection failed due to port and ip problems\n");
        return 1;
    }

    write(CreateSocket,dataSending, strlen(dataSending));
 
    if((readLen = read(CreateSocket, dataReceived, sizeof(dataReceived)-1)) > 0)
    {
        dataReceived[readLen] = 0;
        if(fputs(dataReceived, stdout) == EOF)
        {
            printf("\nStandard output error");
        }
 
        printf("\n");
    }
    else
    {
        printf("Standard input error \n");
    }
 
    return 0;
}

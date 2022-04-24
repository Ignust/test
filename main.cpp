
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include<windows.h>
#include<winsock2.h>
#include<ws2tcpip.h>
//#include<winsock.h>

using namespace std;

HANDLE hSerial;
//#define DEFAULT_PORT "27015"

int main()
{
    ADDRINFO hints;
    ADDRINFO* addrResult = NULL;
    SOCKET ConnectSocket = INVALID_SOCKET;

    WSADATA wsaData;
    int resalt;
    resalt = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(resalt) {
        cout << "WSAStartup failed, resalt = " << resalt << endl;
        return 1;
    }

   ZeroMemory(&hints,sizeof (hints));
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_protocol = IPPROTO_TCP;

   resalt = getaddrinfo("localhost","666",&hints,&addrResult);
   if(resalt) {
       cout << "getaddrinfo failed, resalt = " << resalt << endl;
       WSACleanup();
       return 1;
   }

   ConnectSocket = socket(addrResult->ai_flags, addrResult->ai_socktype, addrResult->ai_protocol);
   if (ConnectSocket == INVALID_SOCKET){
       cout << "ConnectSocket == INVALID_SOCKET" << endl;
       WSACleanup();
       freeaddrinfo(addrResult);
       return 1;
   }

   resalt = connect(ConnectSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
   if (resalt == SOCKET_ERROR){
       cout << "Unable connect to server" << endl;
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
   }

   const char* sendBuffer = "Hi from Client";
   resalt = send(ConnectSocket, sendBuffer,(int)strlen(sendBuffer),0);
     if(resalt == SOCKET_ERROR){
         cout << "send failed, ERROR" << endl;
         closesocket(ConnectSocket);
         freeaddrinfo(addrResult);
         WSACleanup();
         return 1;
     }
     cout << "Bytes send: " << resalt << endl;

     resalt = shutdown(ConnectSocket, SD_SEND);
     if (resalt == SOCKET_ERROR){
         cout << "Sshutdown ERROR" << endl;
          closesocket(ConnectSocket);
          freeaddrinfo(addrResult);
          WSACleanup();
          return 1;
     }

     char recvBuffer[512];

     do {
         ZeroMemory(recvBuffer,512);
         resalt = recv(ConnectSocket,recvBuffer,512,0);
         if(resalt > 0){
             cout << "Recived " << resalt <<" bytes" << endl;
             cout << "Recived data: " << recvBuffer << endl;
         }
     } while (resalt > 0);

     closesocket(ConnectSocket);
     freeaddrinfo(addrResult);
     WSACleanup();
    return 0;
}



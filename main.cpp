
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
    SOCKET ClientSocket = INVALID_SOCKET;
    SOCKET ListenSocket = INVALID_SOCKET;

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
   hints.ai_flags = AI_PASSIVE;

   resalt = getaddrinfo(NULL,"666",&hints,&addrResult);
   if(resalt) {
       cout << "getaddrinfo failed, resalt = " << resalt << endl;
       WSACleanup();
       return 1;
   }

   ListenSocket = socket(addrResult->ai_flags, addrResult->ai_socktype, addrResult->ai_protocol);
   if (ListenSocket == INVALID_SOCKET){
       cout << "ListenSocket == INVALID_SOCKET" << endl;
       WSACleanup();
       freeaddrinfo(addrResult);
       return 1;
   }

   resalt = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
   if (resalt == SOCKET_ERROR){
       cout << "Binding socket failed" << endl;
        closesocket(ListenSocket);
        ListenSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
   }

    resalt = listen(ListenSocket, SOMAXCONN);
    if (resalt == SOCKET_ERROR){
        cout << "Listenend socket failed" << endl;
         closesocket(ListenSocket);
         ListenSocket = INVALID_SOCKET;
         freeaddrinfo(addrResult);
         WSACleanup();
         return 1;
    }

    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == SOCKET_ERROR){
        cout << "Accepting socket failed" << endl;
         closesocket(ListenSocket);
         ListenSocket = INVALID_SOCKET;
         freeaddrinfo(addrResult);
         WSACleanup();
         return 1;
    }



     char recvBuffer[512];

     do {
         ZeroMemory(recvBuffer,512);
         resalt = recv(ClientSocket,recvBuffer,512,0);
         if(resalt > 0){
             cout << "Recived " << resalt <<" bytes" << endl;
             cout << "Recived data: " << recvBuffer << endl;

             const char* sendBuffer = "Hi from Server";
             resalt = send(ClientSocket, sendBuffer,(int)strlen(sendBuffer),0);
               if(resalt == SOCKET_ERROR){
                   cout << "send failed, ERROR" << endl;
                   closesocket(ClientSocket);
                   freeaddrinfo(addrResult);
                   WSACleanup();
                   return 1;
               }
               cout << "Bytes send: " << resalt << endl;
         }
     } while (resalt > 0);

     resalt = shutdown(ClientSocket, SD_SEND);
     if (resalt == SOCKET_ERROR){
         cout << "Sshutdown ClientSocket ERROR" << endl;
          closesocket(ClientSocket);
          freeaddrinfo(addrResult);
          WSACleanup();
          return 1;
     }

     closesocket(ClientSocket);
     freeaddrinfo(addrResult);
     WSACleanup();

     closesocket(ListenSocket);
    return 0;
}



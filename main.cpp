#include"mail.hpp"
#include <iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/epoll.h>
#include <cstring>


using namespace std;

int main()
{

    cout << "test" << endl;
    int masterSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in mSockAddr;
    mSockAddr.sin_family = AF_INET;
    mSockAddr.sin_port = htons(12345);
    mSockAddr.sin_addr.s_addr = htons(INADDR_ANY);

    connect(masterSocket, (sockaddr*)(&mSockAddr), sizeof (mSockAddr));


    int epollFd =epoll_create1(0);
    epoll_event mEvent;
    epoll_event mEvents[5];
    mEvent.events = EPOLLIN;
    mEvent.data.fd = masterSocket;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, masterSocket, &mEvent);
    epoll_event mEvent0;
    mEvent0.data.fd = 0;
    mEvent0.events = EPOLLIN;
    epoll_ctl(epollFd, EPOLL_CTL_ADD, 0, &mEvent0);
    /*
    int epollFd_Out = epoll_create1(0);
    epoll_event mEvent_Out;
    mEvent_Out.events = EPOLLIN;
    mEvent_Out.data.fd = 0;
    epoll_ctl(epollFd_Out, EPOLL_CTL_ADD, 0, &mEvent_Out);
    */
    while (1) {



        int namberEpoll = epoll_wait(epollFd, mEvents,5, 0);
        for(int i = 0; i < namberEpoll; ++i){
            if(mEvents[i].data.fd == masterSocket){
                Mail tempMail;
                tempMail.data[0]=0;
                int namberRead =read(mEvents[i].data.fd, &tempMail.data, sizeof (tempMail.data));
                if(namberRead > 0){
                    cout << i << ": read: " << tempMail.data << endl;
                }
            }
            else {
                Mail tempMail;
                memset(tempMail.data, 0, sizeof(tempMail.data));
                tempMail.data[0] = 0;
                tempMail.typeMail = MESSAGE;
                read(mEvents[i].data.fd, tempMail.data,sizeof(tempMail.data));
                if(tempMail.data[0]!=0){
                    cout <<" tempMail.data: " << tempMail.data << endl;
                    send(masterSocket, &tempMail, sizeof (tempMail), 0);
                }
            }

        }

        /*
        char buf[1024] = {0};
        int N = epoll_wait(epollFd_Out, mEvents, 5 , 0);
        for(int a =0; a < N; ++a){
            Mail tempMail;
            memset(tempMail.data, 0, sizeof(tempMail.data));
            tempMail.data[0] = 0;
            tempMail.typeMail = MESSAGE;
            read(mEvents[a].data.fd, tempMail.data,sizeof(tempMail.data));
            if(tempMail.data[0]!=0){
                cout <<" tempMail.data: " << tempMail.data << endl;
                send(masterSocket, &tempMail, sizeof (tempMail), 0);
            }

        }
        */
    }






    close(masterSocket);

    return 0;
}

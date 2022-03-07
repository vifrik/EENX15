//
// Created by Viktor on 2022-02-22.
//
#ifdef TCP
#ifndef ARUCOCPP_TCPCLIENT_H
#define ARUCOCPP_TCPCLIENT_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <errno.h>
#include <thread>
#include <mutex>
#include <atomic>

class tcpclient {
private:
    bool _isConnected;
    bool _isClosed;
    int _sockfd;
    struct sockaddr_in _server;

    void setAddress(const std::string& address, int port);
    void initializeSocket();
    void close();
protected:
public:
    tcpclient();
    ~tcpclient();
    void connectTo(const std::string & address, int port);
    void sendMsg(const char * msg, size_t size);


};


#endif //ARUCOCPP_TCPCLIENT_H
#endif // TCP
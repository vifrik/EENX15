//
// Created by Viktor on 2022-02-22.
//
#ifdef TCP

#include "tcpclient.h"

tcpclient::tcpclient() {
    _isConnected = false;
    _isClosed = true;
}

tcpclient::~tcpclient() {
    close();
}

void tcpclient::connectTo(const std::string &address, int port) {
    try {
        initializeSocket();
        setAddress(address, port);
    } catch (const std::runtime_error& error) {
        std::cerr << "Runtime error";
        return;
    }

    const int connectResult = connect(_sockfd , (struct sockaddr *)&_server , sizeof(_server));
    const bool connectionFailed = (connectResult == -1);
    if (connectionFailed) {
        std::cerr << "Connection failed";
        return;
    }

    _isConnected = true;
    _isClosed = false;
}

void tcpclient::initializeSocket() {
    _sockfd = socket(AF_INET , SOCK_STREAM , 0);
    const bool socketFailed = (_sockfd == -1);
    if (socketFailed) {
        throw std::runtime_error(strerror(errno));
    }
}

void tcpclient::setAddress(const std::string& address, int port) {
    const int inetSuccess = inet_aton(address.c_str(), &_server.sin_addr);

    if(!inetSuccess) { // inet_addr failed to parse address
        // if hostname is not in IP strings and dots format, try resolve it
        struct hostent *host;
        struct in_addr **addrList;
        if ( (host = gethostbyname( address.c_str() ) ) == nullptr){
            throw std::runtime_error("Failed to resolve hostname");
        }
        addrList = (struct in_addr **) host->h_addr_list;
        _server.sin_addr = *addrList[0];
    }
    _server.sin_family = AF_INET;
    _server.sin_port = htons(port);
}


void tcpclient::sendMsg(const char * msg, size_t size) {
    const size_t numBytesSent = send(_sockfd, msg, size, 0);

    if (numBytesSent < 0 ) { // send failed
        std::cerr << "oh no";
        return;
    }
    if (numBytesSent < size) { // not all bytes were sent
        char errorMsg[100];
        sprintf(errorMsg, "Only %lu bytes out of %lu was sent to client", numBytesSent, size);
        std::cerr << "oh no";
        return;
    }
}

void tcpclient::close(){
    if (_isClosed) {
        std::cerr << "oh no";
        return;
    }

    const bool closeFailed = (::close(_sockfd) == -1);
    if (closeFailed) {
        std::cerr << "oh no";
        return;
    }
    _isClosed = true;
}
#endif // TCP

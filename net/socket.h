#pragma once
#ifdef _WIN32
    #include <winsock2.h?
    typedef int socklen_t;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <fcntl.h>
    #include <unistd.h>
#endif

class UDPSocket {
    public:
        UDPSocket();
        ~UDPSocket();

        bool Bind(unsigned short port);
        bool SetNonBlocking();
        int SendTo(const void* data, int size, sockaddr_in& address);
        int ReceiveFrom(void* data, int size, sockaddr_in& address);

    private:
        int handle;
}
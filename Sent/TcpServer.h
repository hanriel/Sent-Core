//
// Created by Данил Федосеев on 14.02.2021.
//

#ifndef COREPROJECT_TCPSERVER_H
#define COREPROJECT_TCPSERVER_H

#include <cstdint>
#include <functional>
#include <thread>
#include <list>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

static constexpr uint16_t buffer_size = 4096;

class TcpServer {
public:
    TcpServer();

    class Client;

    typedef std::function<void(Client)> handler_function_t;

    //Статус сервера
    enum class status : uint8_t {
        up = 0,
        err_socket_init = 1,
        err_socket_bind = 2,
        err_socket_listening = 3,
        close = 4
    };

    TcpServer(handler_function_t handler);

    ~TcpServer();

    //! Set client handler
    void setHandler(handler_function_t handler);

    status getStatus() const { return _status; }

    status startServer();

    status restartServer();

    void stopServer();

    void joinLoop();


private:
    status _status = status::close;
    handler_function_t handler;

    std::thread handler_thread;
    std::list<std::thread> client_handler_threads;
    std::list<std::thread::id> client_handling_end;

    int serv_socket;

    void handlingLoop();
};

class TcpServer::Client {
    int socket;
    struct sockaddr_in address;
    char buffer[buffer_size];

public:
    Client(int socket, struct sockaddr_in address);

public:
    Client(const Client &other);

    ~Client();

    uint32_t getHost() const;

    uint16_t getPort() const;

    void connectTo(Client &other_client) const;

    void waitConnect() const;

    int loadData();

    char *getData();

    bool sendData(const char *buffer, const size_t size) const;

};


#endif //COREPROJECT_TCPSERVER_H

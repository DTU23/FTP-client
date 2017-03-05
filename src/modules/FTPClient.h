#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
//System Imports
#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <fstream>
#include <cassert>
// User Imports
#include "Helper.h"

using namespace std;

class FTPClient
{
private:
    string server_ip;
    uint16_t port, data_port;
    string user;
    string password;
    struct sockaddr_in server;
    int sock;
    bool create_socket(int *sock);
    bool open_connection(int *sock);
    bool send_cmd(int *sock, string message);
    string get_response(int *sock);
    void close_socket(int *sock);
    void set_data_port_number(string msg_227);
public:
    FTPClient(string server_ip, uint16_t port, string user, string password);
    ~FTPClient(void);
    uint16_t get_data_port_number();
    bool create_socket();
    bool open_connection();
    bool send_cmd(string message);
    bool enter_passive_mode();
    string get_response();
};

#endif // CLIENTSOCKET_H
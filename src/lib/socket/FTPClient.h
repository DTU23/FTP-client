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
#include <unistd.h>
#include <netdb.h>
#include <unistd.h>
#include <fstream>
// User Imports
#include "../Helper.h"

using namespace std;

class FTPClient
{
private:
    string server_ip;
    uint16_t port;
    string user;
    string password;
    bool passive_mode;
    struct sockaddr_in server;
    int sock;
public:
    FTPClient(string server_ip, uint16_t port, string user, string password, bool passive_mode);
    uint16_t get_port_number(string msg_227);
    bool create_socket();
    bool create_socket(int *sock);
    bool open_connection();
    bool open_connection(int *sock);
    bool send_cmd(string message);
    bool send_cmd(int *sock, string message);
    string get_response();
    string get_response(int *sock);
};

#endif // CLIENTSOCKET_H
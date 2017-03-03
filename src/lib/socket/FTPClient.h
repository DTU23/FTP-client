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
#include <cassert>
#include <experimental/filesystem>

#define SERVER_IP "130.226.195.126"
#define FTP_PORT 21

using namespace std;

class FTPClient
{
private:
    int test;
    struct sockaddr_in server;
    FILE *received_file;
public:
    int sock_telnet, sock_ftp;
    FTPClient();
    uint16_t get_port_number(string msg_227);
    void raiseError(string message);
    bool create_socket(int *sock);
    bool open_connection(int *sock, uint16_t port, string ip);
    bool send_cmd(int *sock, string message);
    void receive_file(int *sock, string file_path);
    string receive_response(int *sock);
    void print_message(string message);
};

#endif // CLIENTSOCKET_H
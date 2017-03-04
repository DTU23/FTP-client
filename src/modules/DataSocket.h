#ifndef DataSocket_H
#define DataSocket_H
// System Imports
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
#include "Helper.h"

using namespace std;

class DataSocket {
private:
    string server_ip;
    uint16_t port;
    struct sockaddr_in server;
    int sock;
    string home_path;
    bool create_socket(int *sock);
    bool open_connection(int *sock);
    void receive_file(int *sock, string file_name);
    void send_file(int *sock, string file_name, string upload_path);
    void close_socket(int *sock);
public:
    DataSocket(string server_ip, uint16_t port);
    ~DataSocket(void);
    bool create_socket();
    bool open_connection();
    void receive_file(string file_name);
    void send_file(string file_name);
    void send_file(string file_name, string upload_path);
    void close_socket();
};


#endif //DataSocket_H

#ifndef DATARECEIVESOCKET_H
#define DATARECEIVESOCKET_H
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
#include "../Helper.h"

using namespace std;

class DataReceiveSocket {
private:
    string server_ip;
    uint16_t port;
    struct sockaddr_in server;
    int sock;
    string home_path;
public:
    DataReceiveSocket(string server_ip, uint16_t port);
    bool create_socket();
    bool create_socket(int *sock);
    bool open_connection();
    bool open_connection(int *sock);
    void receive_file(string file_name);
    void receive_file(int *sock, string file_name);
};


#endif //DATARECEIVESOCKET_H

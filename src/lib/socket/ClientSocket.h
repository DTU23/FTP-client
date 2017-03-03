#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <unistd.h>
#include <fstream>
#include <cassert>

using namespace std;

uint16_t get_port_number(string msg_227);

bool create_socket(int *sock);
bool open_connection(int *sock, uint16_t port, string ip);
bool send_cmd(int *sock, string message);

void receive_data(int *sock);
string receive_response(int *sock);

void raiseError(string message);
void print_message(string message);

int runClient();


#endif // CLIENTSOCKET_H

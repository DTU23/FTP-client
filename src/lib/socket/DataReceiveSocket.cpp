#include "DataReceiveSocket.h"
using namespace std;
/**
 * Constructor
 * @param server_ip
 * @param port
 */
DataReceiveSocket::DataReceiveSocket(string server_ip, uint16_t port) {
    this->server_ip = server_ip;
    this->port = port;
    // Get environment variable for HOME-folder-path based off Operating system
    string home_path = "";
    #ifdef _WIN32
        this->home_path = getenv("HOMEPATH");
    #else
        this->home_path = getenv("HOME");
    #endif
}

/**
 * Method for creating a socket from socket pointer
 * @param sock
 * @return boolean for success or error
 */
bool DataReceiveSocket::create_socket(int *sock) {
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == -1) {
        return false;
    }
    return true;
}
// Overload method
bool DataReceiveSocket::create_socket() {
    DataReceiveSocket::create_socket(&this->sock);
}

/**
 * Method for opening a socket-connection to remote server
 * @param sock
 * @param port
 * @param ip
 * @return boolean for success or error
 */
bool DataReceiveSocket::open_connection(int *sock) {
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(&this->server_ip[0]);
    if (connect(*sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        return false;
    }
    return true;
}
// Overload method
bool DataReceiveSocket::open_connection() {
    return DataReceiveSocket::open_connection(&this->sock);
}

/**
 * Method for receiving file-data
 * @param sock
 */
void DataReceiveSocket::receive_file(int *sock, string file_name) {
    char buffer[BUFSIZ];
    FILE *received_file;

    /* Store filesize in variable */
    size_t n = recv(*sock, buffer, BUFSIZ, 0);

    // Open file for writing
    string file = this->home_path+"/Desktop/"+file_name;
    received_file = fopen(file.c_str(), "w");

    // Rais error if we can't open file
    if (received_file == NULL) {
        Helper::raiseError("Failed to open file!");
    }

    // Write buffer to file
    fwrite(buffer, sizeof(char), n, received_file);

    // Close file and socket
    fclose(received_file);
    close(*sock);
}
// Overload method
void DataReceiveSocket::receive_file(string file_name) {
    DataReceiveSocket::receive_file(&this->sock, file_name);
}
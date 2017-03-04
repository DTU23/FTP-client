#include "DataSocket.h"
using namespace std;
/**
 * Constructor
 * @param server_ip
 * @param port
 */
DataSocket::DataSocket(string server_ip, uint16_t port) {
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
 * Destructor
 */
DataSocket::~DataSocket(int *sock) {
    close(*sock);
}
// Overload method
DataSocket::~DataSocket(void) {
    DataSocket::~DataSocket(&this->sock);
}

/**
 * Method for creating a socket from socket pointer
 * @param sock
 * @return boolean for success or error
 */
bool DataSocket::create_socket(int *sock) {
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == -1) {
        return false;
    }
    return true;
}
// Overload method
bool DataSocket::create_socket() {
    DataSocket::create_socket(&this->sock);
}

/**
 * Method for opening a socket-connection to remote server
 * @param sock
 * @param port
 * @param ip
 * @return boolean for success or error
 */
bool DataSocket::open_connection(int *sock) {
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(&this->server_ip[0]);
    if (connect(*sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        return false;
    }
    return true;
}
// Overload method
bool DataSocket::open_connection() {
    return DataSocket::open_connection(&this->sock);
}

/**
 * Method for receiving file-data
 * @param sock
 */
void DataSocket::receive_file(int *sock, string file_name) {
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
}
// Overload method
void DataSocket::receive_file(string file_name) {
    DataSocket::receive_file(&this->sock, file_name);
}

/**
 * Method for sending file
 * @param sock pointer to an open socket
 * @param file_name name of file to be uplaoded
 * @param upload_path path where file should be uploaded
 */
void DataSocket::send_file(int *sock, string file_name, string upload_path) {
    char buffer[BUFSIZ];
    FILE *send_file;

    // Open file for writing
    string file = this->home_path+"/Desktop/"+file_name;

    // Raise error if we can't open file
    if (send_file == NULL) {
        Helper::raiseError("Failed to open file!");
    }

    ifstream infile (file,ifstream::binary);

    // get size of file
    infile.seekg (0,infile.end);
    long size = infile.tellg();
    infile.seekg (0);

    // read content of infile
    infile.read (buffer,size);
    write(*sock,buffer, strlen(buffer));

    int n = send(*sock, &buffer[0], size, 0);
    if (n < 1){
        Helper::raiseError("File not uploaded!");
    }
}
// Overload method
void DataSocket::send_file(string file_name, string upload_path) {
    DataSocket::send_file(&this->sock, file_name, upload_path);
}
// Overload method
void DataSocket::send_file(string file_name) {
    DataSocket::send_file(file_name, "/");
}
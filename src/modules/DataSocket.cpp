#include "DataSocket.h"
#define BUFFER_SIZE 1024
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
    // create new socket
    if (!this->create_socket()) {
        Helper::raiseError("Couldn't create Data transfer socket!");
    }
    Helper::print_message("Data transfer socket created");

    // open socket connection
    if (!this->open_connection()) {
        Helper::raiseError("Couldn't open connection!");
    }

}
/**
 * Destructor
 */
// Overload method
DataSocket::~DataSocket(void) {
    Helper::print_message("Closing Data Socket!");
    DataSocket::close_socket(&this->sock);
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
    char buffer[BUFFER_SIZE];
    FILE *received_file;
    // Open file for writing
    string file = this->home_path+"/Desktop/"+file_name;
    received_file = fopen(file.c_str(), "w");

    // Raise error if we can't open file
    if (received_file == NULL) {
        Helper::raiseError("Failed to open file!");
    }
    /* Store filesize in variable */
    int chunk = recv(*sock, buffer, BUFFER_SIZE, MSG_PEEK);

    // Write large files in chunks
    if(chunk >= BUFFER_SIZE){
        Helper::print_message("Too Large!");
        do{
            chunk = recv(*sock, buffer, chunk, 0);
            fwrite(buffer, sizeof(char), chunk, received_file);
        }while (chunk > 0);
    // Smaller files can be written directly
    }else{
        // Write buffer to file
        fwrite(buffer, sizeof(char), chunk, received_file);
    }
    // Close file
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
    char buffer[BUFFER_SIZE];
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

/**
 * Method for closing socket (called by destructor)
 * @param sock
 */
void DataSocket::close_socket(int *sock) {
    close(*sock);
}
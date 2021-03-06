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
    this->sock = 0;
    this->server.sin_family = AF_INET;  // IP-protocol family defined from socket-library
    this->server.sin_port = htons(this->port);
    //Convert Internet host address from numbers-and-dots notation in CP. into binary data in network byte order with inet_addr() method
    this->server.sin_addr.s_addr = inet_addr(&this->server_ip[0]);
    // Get environment variable for HOME-folder-path based off Operating system
    string home_path = "";
    #ifdef _WIN32
        this->home_path = getenv("HOMEPATH");
    #else
        this->home_path = getenv("HOME");
    #endif
    // Create socket
    if(this->create_socket() < 0){
        Helper::raiseError("Erorr creating socket");
    }
    Helper::print_message("Data socket created!");
    // Open connection
    if(this->open_connection() < 0){
        Helper::raiseError("Error opening connection!");
    }
    Helper::print_message("Data socket connection opened!");
}
/**
 * Destructor
 */
DataSocket::~DataSocket(void) {
    DataSocket::close_socket();
}

/**
 * Method for creating a socket from socket pointer
 * @param sock
 * @return boolean for success or error
 */
bool DataSocket::create_socket(int *sock) {
    // Create socket on received pointer-address
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == -1) {
        return false;
    }
    return true;
}
// Overload method
bool DataSocket::create_socket() {
    return DataSocket::create_socket(&this->sock);
}

/**
 * Method for opening a socket-connection to remote server
 * @param sock
 * @param port
 * @param ip
 * @return boolean for success or error
 */
bool DataSocket::open_connection(int *sock) {
    // invoke socket.h connect() method on pointer address
    if (connect(*sock, (struct sockaddr *) &this->server, sizeof(server)) < 0) {
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
string DataSocket::receive_file(int *sock, string file_name) {
    // Create empty receive buffer and a file-type pointer
    char buffer[BUFFER_SIZE];
    FILE *received_file;
    // Open file for writing
    string file = this->home_path+"/Desktop/"+file_name;
    // set pointer address as a filestream
    received_file = fopen(file.c_str(), "w");
    // Raise error if we can't open file
    if (received_file == NULL) {
        Helper::raiseError("Failed to open file!");
    }
    // Store first chunk of file
    int chunk = recv(*sock, buffer, BUFFER_SIZE, 0);
    if(chunk > BUFFER_SIZE){
        // Smaller files can be written directly
        fwrite(buffer, sizeof(char), chunk, received_file);
    }else{
        do{
            // Write buffer to file until it's empty
            fwrite(buffer, sizeof(char), chunk, received_file);
            // Retrieve next chunk of file
            chunk = recv(*sock, buffer, chunk, 0);
        }while (chunk > 0);
    }
    // Close file
    fclose(received_file);
    // Return filepath (to be used by printer function)
    return this->home_path+"/Desktop/"+file_name;
}
// Overload method
string DataSocket::receive_file(string file_name) {
    return DataSocket::receive_file(&this->sock, file_name);
}

/**
 * Method for sending file
 * @param sock pointer to an open socket
 * @param file_name name of file to be uplaoded
 * @param upload_path path where file should be uploaded
 */
void DataSocket::send_file(int *sock, string file_name, string upload_path) {
    // Create empty receive buffer and a file-type pointer
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
    // Write it to datasocket
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
    if(close(*sock) < 0){
        Helper::raiseError("Could not close data socket!");
    }
    Helper::print_message("Data socket closed!");
}
// Overload method
void DataSocket::close_socket() {
    Helper::print_message("Closing data socket...");
    DataSocket::close_socket(&this->sock);
}
#include "FTPClient.h"

/**
 * Constructor method
 */
FTPClient::FTPClient(string server_ip, uint16_t port, string user, string password, bool passive_mode) {
    this->server_ip = server_ip;
    this->port = port;
    this->user = user;
    this->password = password;
    this->passive_mode = passive_mode;
    this->server.sin_family = AF_INET;
    this->server.sin_port = htons(port);
    this->server.sin_addr.s_addr = inet_addr(&this->server_ip[0]);
}


/**
 * Destructor
 */
FTPClient::~FTPClient(int *sock) {
    close(*sock);
}
// Overload method
FTPClient::~FTPClient(void) {
    FTPClient::~FTPClient(&this->sock);
}

/**
 * Method will take a string-response for passive-mode-entry and split it out into segments and calculate port-number
 * @param msg_227
 * @return port number as unsigned integer
 */
uint16_t FTPClient::get_port_number(string msg_227) {
    // Split string where parenthesis starts and ends
    unsigned long ip_start = msg_227.find('(');
    unsigned long ip_end = msg_227.find(')');
    string return_ip = msg_227.substr(ip_start + 1, ip_end - ip_start - 1);

    // Create stringstrea from ip-string
    stringstream ss(return_ip);
    string segment;
    vector<string> seglist;

    // Split string into segmentlist with ','-delimiters
    while (getline(ss, segment, ',')) {
        seglist.push_back(segment);
    }

    // Return port-number calculation
    return atoi(seglist[4].c_str()) * 256 + atoi(seglist[5].c_str());
}


/**
 * Method for creating a socket from socket pointer
 * @param sock
 * @return boolean for success or error
 */
bool FTPClient::create_socket(int *sock) {
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == -1) {
        return false;
    }
    return true;
}
// Overload method
bool FTPClient::create_socket() {
    FTPClient::create_socket(&this->sock);
}

/**
 * Method for opening a socket-connection to remote server
 * @param sock
 * @param port
 * @param ip
 * @return boolean for success or error
 */
bool FTPClient::open_connection(int *sock) {
    if (connect(*sock, (struct sockaddr *) &this->server, sizeof(this->server)) < 0) {
        return false;
    }
    return true;
}
// Overload method
bool FTPClient::open_connection() {
    return FTPClient::open_connection(&this->sock);
}

/**
 * Method for sending a command to an open socket-connection
 * @param sock
 * @param message
 * @return boolean for success or error
 */
bool FTPClient::send_cmd(int *sock, string message) {
    if (send(*sock, message.c_str(), strlen(message.c_str()), 0) < 0) {
        return false;
    }
    Helper::print_message(message);
    return true;
}
// Overload method
bool FTPClient::send_cmd(string message) {
    return FTPClient::send_cmd(&this->sock, message);
}

/**
 * Method for receiving socket-response from server
 * @param sock
 * @return
 */
string FTPClient::get_response(int *sock) {
    char buffer[BUFSIZ];
    string reply;
    int n = recv(*sock, &buffer[0], BUFSIZ, 0);
    if (n < 0) {
        Helper::raiseError("Data receive failed!");
    }
    buffer[n] = '\0';
    return buffer;
}
// Overload method
string FTPClient::get_response() {
    return FTPClient::get_response(&this->sock);
}
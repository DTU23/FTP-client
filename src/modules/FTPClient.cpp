#include "FTPClient.h"
#define BUFFER_SIZE 1024

/**
 * Constructor method
 */
FTPClient::FTPClient(string server_ip, uint16_t port, string user, string password) {
    this->server_ip = server_ip;
    this->port = port;
    this->user = user;
    this->password = password;
    this->server.sin_family = AF_INET;
    this->server.sin_port = htons(port);
    this->server.sin_addr.s_addr = inet_addr(&this->server_ip[0]);

    // Create socket
    if (!this->create_socket()) {
        Helper::raiseError("Couldn't create socket!");
    }
    Helper::print_message("FTP Socket Created!");

    // Open connection
    if (!this->open_connection()) {
        Helper::raiseError("Couldn't open connection!");
    }
    Helper::print_message("FTP Connection successful!");
    // Empty out response buffer for empty messages
    Helper::print_message(this->get_response());
    // Send initial hello command, to start communications
    if (!this->send_cmd("hello\r\n")) {
        Helper::raiseError("Data send error!");
    }
    Helper::print_message(this->get_response());

    // Set user anonymous
    string user_cmd = "USER "+this->user+"\r\n";
    if (!this->send_cmd(user_cmd)) {
        Helper::raiseError("Data send error!");
    }
    // Get response message for USER command
    Helper::print_message(this->get_response());

    string pass_cmd = "PASS "+this->password+"\r\n";
    // Send password (can be anything)
    if (!this->send_cmd(pass_cmd)) {
        Helper::raiseError("Data send error!");
    }
    Helper::print_message(this->get_response());
}


/**
 * Destructor
 */
FTPClient::~FTPClient(void) {
    FTPClient::close_socket();
}

/**
 * Method for getting data-port calculated by set_data_port_number method
 * @return data_port number
 */
uint16_t FTPClient::get_data_port_number(){
    // assert used to avoid NULL arithmatic
    assert(this->data_port != NULL);
    if(this->data_port==0){
        Helper::raiseError("Data port not calculated!");
    }
    return this->data_port;
}

/**
 * Method will take a string-response for passive-mode-entry and split it out into segments and calculate port-number
 * @param msg_227
 */
void FTPClient::set_data_port_number(string msg_227) {
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

    // Set data port number
    this->data_port = atoi(seglist[4].c_str()) * 256 + atoi(seglist[5].c_str());
}


/**
 * Method for creating a socket on socket pointer
 * @param sock
 * @return boolean for success or error
 */
bool FTPClient::create_socket(int *sock) {
    // Create socket on received pointer-address
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock == -1) {
        return false;
    }
    return true;
}
// Overload method
bool FTPClient::create_socket() {
    return FTPClient::create_socket(&this->sock);
}

/**
 * Method for opening a socket-connection to remote server
 * @param sock
 * @return boolean for success or error
 */
bool FTPClient::open_connection(int *sock) {
    // invoke socket.h connect() method on pointer address
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
 * @param sock pointer
 * @param message string
 * @return boolean for success or error
 */
bool FTPClient::send_cmd(int *sock, string message) {
    // invoke socket.h send() method on pointer address
    if (send(*sock, message.c_str(), strlen(message.c_str()), 0) < 0) {
        return false;
    }
    Helper::print_message("request: "+message.substr(0, message.size()-1));
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
    // Create a receive buffer and response string (empty)
    char buffer[BUFFER_SIZE];
    string reply;
    // Get size of received packet & write it to the buffer
    int n = recv(*sock, &buffer[0], BUFFER_SIZE, 0);
    if (n < 0) {
        Helper::raiseError("Data receive failed!");
    }
    // put null-termination on the last place in the buffer to act like a string
    buffer[n-1] = '\0';
    // Convert buffer to string
    string buff(buffer);
    // Build reply message string
    reply = "response: " + buff;
    // Return reply
    return reply;
}
// Overload method
string FTPClient::get_response() {
    return FTPClient::get_response(&this->sock);
}

/**
 * Method for entering passive mode
 * @return
 */
bool FTPClient::enter_passive_mode(){
    // Enter passive mod
    if (!this->send_cmd("PASV\r\n")) {
        Helper::raiseError("Unable to enter passive mode!");
    }
    // Translate passive mode response to socket port-number
    string response = this->get_response();
    // Print response
    Helper::print_message(response);
    // Apply new dataport for passive mode
    this->set_data_port_number(response);
    return true;
}
/**
 * Method for closing socket (called by destructor)
 * @param sock
 */
void FTPClient::close_socket(int *sock) {
    if(close(*sock) < 0){
        Helper::raiseError("Could not close FTP socket!");
    }
    Helper::print_message("FTP socket closed!");
}
// Overload method
void FTPClient::close_socket() {
    Helper::print_message("Closing FTP socket...");
    FTPClient::close_socket(&this->sock);
}
#include "ClientSocket.h"

/**
 * Constructor method
 */
FTPClient::FTPClient() {

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
 * Method will print an error message and exit program
 * @param message
 */
void FTPClient::raiseError(string message) {
    print_message(message);
    exit(1);
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
/**
 * Method for opening a socket-connection to remote server
 * @param sock
 * @param port
 * @param ip
 * @return boolean for success or error
 */
bool FTPClient::open_connection(int *sock, uint16_t port, string ip) {
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(&ip[0]);
    if (connect(*sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        return false;
    }
    return true;
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
    print_message(message);
    return true;
}

/**
 * Method for receiving file-data
 * @param sock
 */
void FTPClient::receive_file(int *sock, string file_path) {
    char buffer[BUFSIZ];
    FILE *received_file;

    /* Store filesize in variable */
    size_t n = recv(*sock, buffer, BUFSIZ, 0);

    // Open file for writing
    received_file = fopen(file_path.c_str(), "w");

    // Rais error if we can't open file
    if (received_file == NULL) {
        raiseError("Failed to open file!");
    }

    // Write buffer to file
    fwrite(buffer, sizeof(char), n, received_file);

    // Close file and socket
    fclose(received_file);
    close(*sock);
}

/**
 * Method for receiving socket-response from server
 * @param sock
 * @return
 */
string FTPClient::receive_response(int *sock) {
    char buffer[BUFSIZ];
    string reply;
    int n = recv(*sock, &buffer[0], BUFSIZ, 0);
    if (n < 0) {
        raiseError("Data receive failed!");
    }
    buffer[n] = '\0';
    return buffer;
}

/**
 * General method for printing messages to the console
 * @param message
 */
void FTPClient::print_message(string message) {
    cout << message << endl;
}
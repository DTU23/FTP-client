#include "ClientSocket.h"

#define BUFSIZ 5000
#define SERVER_IP "130.226.195.126"
#define FTP_PORT 21
int sock_telnet, sock_ftp;
struct sockaddr_in server;
FILE *received_file;

/**
 * Method to start the process of opening socket connections and downloading file
 * @return
 */
int runClient() {
    // Create socket
    if (!create_socket(&sock_telnet)) {
        raiseError("Couldn't create socket!");
    }
    print_message("Socket Created!");

    // Open connection
    if (!open_connection(&sock_telnet, FTP_PORT, SERVER_IP)) {
        raiseError("Couldn't open connection!");
    }
    print_message("Connection successful!");

    // Send initial hello command, to start communications
    if (!send_cmd(&sock_telnet, "hello\n")) {
        raiseError("Data send error!");
    }
    print_message(receive_response(&sock_telnet));

    // Set user anonymous
    if (!send_cmd(&sock_telnet, "USER anonymous\n")) {
        raiseError("Data send error!");
    }
    print_message(receive_response(&sock_telnet));

    // Send password (can be anything)
    if (!send_cmd(&sock_telnet, "PASS pass\n")) {
        raiseError("Data send error!");
    }
    /**
     * Todo: consider why we have to do two receives for it to work.
     */
    print_message(receive_response(&sock_telnet));
    print_message(receive_response(&sock_telnet));

    // Enter passive mod
    if (!send_cmd(&sock_telnet, "PASV\n")) {
        raiseError("Data send error!");
    }
    // Translate passive mode response to socket port #
    uint16_t port = get_port_number(receive_response(&sock_telnet));
    cout << "data-transfer port is: " << port << endl;

    /**
     * Open new socket for FTP
     */
    // Start 2nd socket
    if (!create_socket(&sock_ftp)) {
        raiseError("Couldn't create ftp socket!");
    }
    print_message("FTP socket created");

    // open connection
    if (!open_connection(&sock_ftp, port, SERVER_IP)) {
        raiseError("Couldn't open connection!");
    }
    print_message("connection opened");

    // Send retrieve command
    if (!send_cmd(&sock_ftp, "RETR file.txt")) {
        raiseError("Error receiving file.txt");
    }

    /**
     * Todo: implement file get-method
     */
    receive_data(&sock_ftp);
    print_message("File retrieved");
}

/**
 * Method will take a string-response for passive-mode-entry and split it out into segments and calculate port-number
 * @param msg_227
 * @return port number as unsigned integer
 */
uint16_t get_port_number(string msg_227) {
    unsigned long ip_start = msg_227.find('(');
    unsigned long ip_end = msg_227.find(')');
    string return_ip = msg_227.substr(ip_start + 1, ip_end - ip_start - 1);

    stringstream ss(return_ip);
    string segment;
    vector<string> seglist;

    while (getline(ss, segment, ',')) {
        seglist.push_back(segment);
    }

    return atoi(seglist[4].c_str()) * 256 + atoi(seglist[5].c_str());
}

/**
 * Method will print an error message and exit program
 * @param message
 */
void raiseError(string message) {
    print_message(message);
    exit(1);
}

/**
 * Method for creating a socket
 * @param sock
 * @return boolean for success or error
 */
bool create_socket(int *sock) {
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
bool open_connection(int *sock, uint16_t port, string ip) {
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
bool send_cmd(int *sock, string message) {
    if (send(*sock, message.c_str(), strlen(message.c_str()), 0) < 0) {
        return false;
    }
    return true;
}

/**
 * Method for receiving file-data
 * @param sock
 */
void receive_data(int *sock) {
    ssize_t len;
    char buffer[BUFSIZ];
    int file_size;
    /* Receiving file size */
    recv(*sock, &buffer, BUFSIZ, 0);
    print_message("test");
    file_size = atoi(buffer);
    received_file = fopen("/home/viktor/file.txt", "w");
    if (received_file == NULL) {
        fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));

        exit(EXIT_FAILURE);
    }

    int remain_data = file_size;
    while (((len = recv(*sock, buffer, BUFSIZ, 0)) > 0) && (remain_data > 0)) {
        fwrite(buffer, sizeof(char), len, received_file);
        remain_data -= len;
        fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", len, remain_data);
    }
    fclose(received_file);

}

/**
 * Method for receiving socket-response from server
 * @param sock
 * @return
 */
string receive_response(int *sock) {
    int size = 1024;
    char buffer[size];
    string reply;
    int n = recv(*sock, buffer, sizeof(buffer), 0);
    if (n < 0) {
        raiseError("Data receive failed!");
    }
    buffer[n] = '\0';
    reply = buffer;
    return reply;
}

/**
 * General method for printing messages to the console
 * @param message
 */
void print_message(string message) {
    cout << message << endl;
}
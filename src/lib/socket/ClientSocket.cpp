#include "ClientSocket.h"

#define SERVER_IP "130.226.195.126"
#define FTP_PORT 21
int sock_telnet, sock_ftp;
struct sockaddr_in server;
FILE *received_file;
char receive_buffer[1000000];

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
    if (!send_cmd(&sock_telnet, "hello\r\n")) {
        raiseError("Data send error!");
    }
    print_message(receive_response(&sock_telnet));


    // Set user anonymous
    if (!send_cmd(&sock_telnet, "USER anonymous\r\n")) {
        raiseError("Data send error!");
    }
    // Get response message for USER command
    print_message(receive_response(&sock_telnet));
    // Get 2nd response message asking for password
    print_message(receive_response(&sock_telnet));

    // Send password (can be anything)
    if (!send_cmd(&sock_telnet, "PASS pass\r\n")) {
        raiseError("Data send error!");
    }
    /**
     * Todo: consider why we have to do two receives for it to work.
     */
    print_message(receive_response(&sock_telnet));
    // Enter passive mod
    if (!send_cmd(&sock_telnet, "PASV\r\n")) {
        raiseError("Data send error!");
    }
    // Translate passive mode response to socket port #
    string response = receive_response(&sock_telnet);
    print_message(response);
    uint16_t port = get_port_number(response);
    cout << "data-transfer port is: " << port << endl;

    /**
     * Open new socket for FTP
     */
    // create new socket
    if (!create_socket(&sock_ftp)) {
        raiseError("Couldn't create ftp socket!");
    }
    print_message("FTP socket created");

    // open connection
    if (!open_connection(&sock_ftp, port, SERVER_IP)) {
        raiseError("Couldn't open connection!");
    }

    // Send retrieve command
    if (!send_cmd(&sock_telnet, "RETR file.txt\r\n")) {
        raiseError("Error receiving file.txt");
    }
    // Get environment variable for HOME-folder-path based off Operating system
    string home_path = "";
    #ifdef _WIN32
        home_path = getenv("HOMEPATH");
    #else
        home_path = getenv("HOME");
    #endif
    // Receive file from socket.
    receive_file(&sock_ftp, home_path+"/Desktop/file.txt");
    print_message("File retrieved");
}

/**
 * Method will take a string-response for passive-mode-entry and split it out into segments and calculate port-number
 * @param msg_227
 * @return port number as unsigned integer
 */
uint16_t get_port_number(string msg_227) {
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
void raiseError(string message) {
    print_message(message);
    exit(1);
}

/**
 * Method for creating a socket from socket pointer
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
    print_message(message);
    return true;
}

/**
 * Method for receiving file-data
 * @param sock
 */
void receive_file(int *sock, string file_path) {
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
string receive_response(int *sock) {
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
void print_message(string message) {
    cout << message << endl;
}
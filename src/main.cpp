#include "lib/socket/FTPClient.h"
#include "lib/socket/DataSocket.h"

#define SERVER_IP "130.226.195.126"
#define FTP_PORT 21

int main( int argc, char** argv) {
    FTPClient ftpClient(SERVER_IP, FTP_PORT, "anonymous", "", true);
    // Create socket
    if (!ftpClient.create_socket()) {
        Helper::raiseError("Couldn't create socket!");
    }
    Helper::print_message("Socket Created!");

    // Open connection
    if (!ftpClient.open_connection()) {
        Helper::raiseError("Couldn't open connection!");
    }
    Helper::print_message("Connection successful!");

    // Send initial hello command, to start communications
    if (!ftpClient.send_cmd("hello\r\n")) {
        Helper::raiseError("Data send error!");
    }
    Helper::print_message(ftpClient.receive_response());


    // Set user anonymous
    if (!ftpClient.send_cmd("USER anonymous\r\n")) {
        Helper::raiseError("Data send error!");
    }
    // Get response message for USER command
    Helper::print_message(ftpClient.receive_response());
    // Get 2nd response message asking for password
    Helper::print_message(ftpClient.receive_response());

    // Send password (can be anything)
    if (!ftpClient.send_cmd("PASS pass\r\n")) {
        Helper::raiseError("Data send error!");
    }
    Helper::print_message(ftpClient.receive_response());

    // Enter passive mod
    if (!ftpClient.send_cmd("PASV\r\n")) {
        Helper::raiseError("Data send error!");
    }

    // Translate passive mode response to socket port-number
    string response = ftpClient.receive_response();
    Helper::print_message(response);
    uint16_t port = ftpClient.get_port_number(response);
    cout << "data-transfer port is: " << port << endl;

    /**
     * Open new DataSocket for FTP file transfer
     */
    DataSocket DataSocket(SERVER_IP, port);
    // create new socket
    if (!DataSocket.create_socket()) {
        Helper::raiseError("Couldn't create Data transfer socket!");
    }
    Helper::print_message("Data transfer socket created");

    // open socket connection
    if (!DataSocket.open_connection()) {
        Helper::raiseError("Couldn't open connection!");
    }

    // Send retrieve command on FTPClient
    if (!ftpClient.send_cmd("RETR file.txt\r\n")) {
        Helper::raiseError("Error receiving file.txt");
    }
    // Receive file from socket.
    DataSocket.receive_file("file.txt");
    Helper::print_message("File retrieved");

    // Send store command on FTPClient
    if (!ftpClient.send_cmd("STOR upload.txt\r\n")) {
        Helper::raiseError("Error storing file.txt");
    }
    DataSocket.send_file("send.txt");
    Helper::print_message("File send to FTP server!");
    return 0;
}
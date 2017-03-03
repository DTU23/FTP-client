#include "lib/socket/FTPClient.h"
#include "lib/socket/DataReceiveSocket.h"
#include "lib/Helper.h"

int main( int argc, char** argv) {
    FTPClient ftpClient("130.226.195.126", 21, "anonymous", "", true);
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
    /**
     * Todo: consider why we have to do two receives for it to work.
     */
    Helper::print_message(ftpClient.receive_response());
    // Enter passive mod
    if (!ftpClient.send_cmd("PASV\r\n")) {
        Helper::raiseError("Data send error!");
    }
    // Translate passive mode response to socket port #
    string response = ftpClient.receive_response();
    Helper::print_message(response);
    uint16_t port = ftpClient.get_port_number(response);
    cout << "data-transfer port is: " << port << endl;

    /**
     * Open new socket for FTP
     */
    DataReceiveSocket dataReceiveSocket("130.226.195.126", port);
    // create new socket
    if (!dataReceiveSocket.create_socket()) {
        Helper::raiseError("Couldn't create ftp socket!");
    }
    Helper::print_message("FTP socket created");

    // open connection
    if (!dataReceiveSocket.open_connection()) {
        Helper::raiseError("Couldn't open connection!");
    }

    // Send retrieve command
    if (!ftpClient.send_cmd("RETR file.txt\r\n")) {
        Helper::raiseError("Error receiving file.txt");
    }
    // Receive file from socket.
    dataReceiveSocket.receive_file("file.txt");
    Helper::print_message("File retrieved");
    return 0;
}
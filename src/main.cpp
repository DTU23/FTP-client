#include "./lib/socket/ClientSocket.h"

int main( int argc, char** argv) {
    FTPClient ftpClient;
    // Create socket
    if (!ftpClient.create_socket(&ftpClient.sock_telnet)) {
        ftpClient.raiseError("Couldn't create socket!");
    }
    ftpClient.print_message("Socket Created!");

    // Open connection
    if (!ftpClient.open_connection(&ftpClient.sock_telnet, FTP_PORT, SERVER_IP)) {
        ftpClient.raiseError("Couldn't open connection!");
    }
    ftpClient.print_message("Connection successful!");

    // Send initial hello command, to start communications
    if (!ftpClient.send_cmd(&ftpClient.sock_telnet, "hello\r\n")) {
        ftpClient.raiseError("Data send error!");
    }
    ftpClient.print_message(ftpClient.receive_response(&ftpClient.sock_telnet));


    // Set user anonymous
    if (!ftpClient.send_cmd(&ftpClient.sock_telnet, "USER anonymous\r\n")) {
        ftpClient.raiseError("Data send error!");
    }
    // Get response message for USER command
    ftpClient.print_message(ftpClient.receive_response(&ftpClient.sock_telnet));
    // Get 2nd response message asking for password
    ftpClient.print_message(ftpClient.receive_response(&ftpClient.sock_telnet));

    // Send password (can be anything)
    if (!ftpClient.send_cmd(&ftpClient.sock_telnet, "PASS pass\r\n")) {
        ftpClient.raiseError("Data send error!");
    }
    /**
     * Todo: consider why we have to do two receives for it to work.
     */
    ftpClient.print_message(ftpClient.receive_response(&ftpClient.sock_telnet));
    // Enter passive mod
    if (!ftpClient.send_cmd(&ftpClient.sock_telnet, "PASV\r\n")) {
        ftpClient.raiseError("Data send error!");
    }
    // Translate passive mode response to socket port #
    string response = ftpClient.receive_response(&ftpClient.sock_telnet);
    ftpClient.print_message(response);
    uint16_t port = ftpClient.get_port_number(response);
    cout << "data-transfer port is: " << port << endl;

    /**
     * Open new socket for FTP
     */
    // create new socket
    if (!ftpClient.create_socket(&ftpClient.sock_ftp)) {
        ftpClient.raiseError("Couldn't create ftp socket!");
    }
    ftpClient.print_message("FTP socket created");

    // open connection
    if (!ftpClient.open_connection(&ftpClient.sock_ftp, port, SERVER_IP)) {
        ftpClient.raiseError("Couldn't open connection!");
    }

    // Send retrieve command
    if (!ftpClient.send_cmd(&ftpClient.sock_telnet, "RETR file.txt\r\n")) {
        ftpClient.raiseError("Error receiving file.txt");
    }
    // Get environment variable for HOME-folder-path based off Operating system
    string home_path = "";
    #ifdef _WIN32
        home_path = getenv("HOMEPATH");
    #else
        home_path = getenv("HOME");
    #endif
    // Receive file from socket.
    ftpClient.receive_file(&ftpClient.sock_ftp, home_path+"/Desktop/file.txt");
    ftpClient.print_message("File retrieved");
    return 0;
}
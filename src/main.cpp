#include "modules/FTPClient.h"
#include "modules/DataSocket.h"
#include "modules/FilePrinter.h"
#include <cassert>

#define SERVER_IP "192.168.23.3"
#define FTP_PORT 21

void getData();
void getStatus();

int main( int argc, char** argv) {
    getData();
    getStatus();
}

void getData() {
    FTPClient ftpClient(SERVER_IP, FTP_PORT, "anonymous", "");
    // Enter passive mod on FTP-socket
    ftpClient.enter_passive_mode();
    // Open datasocket on FTP data-port
    DataSocket dataSocket(SERVER_IP, ftpClient.get_data_port_number());
    // Send retrieve data.csv command on FTPClient
    if (!ftpClient.send_cmd("RETR data.csv\r\n")) {
        Helper::raiseError("Error receiving data.csv");
    }
    // Receive file from socket and save it to local drive
    dataSocket.receive_file("data.csv");
}

void getStatus() {
    FTPClient ftpClient(SERVER_IP, FTP_PORT, "anonymous", "");
    // Enter passive mod on FTP-socket
    ftpClient.enter_passive_mode();
    // Open datasocket on FTP data-port
    DataSocket dataSocket(SERVER_IP, ftpClient.get_data_port_number());
    // Send retrieve status.txt command on FTPClient
    if (!ftpClient.send_cmd("RETR status.txt\r\n")) {
        Helper::raiseError("Error receiving status.txt");
    }
    // Receive file from socket and save it to local drive
    dataSocket.receive_file("status.txt");
}
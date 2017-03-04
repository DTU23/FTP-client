# FTP-client
This c++ ftp-client application was developed by:

* Viktor Poulsen
* Christian Niemann
* Frederik Værnegaard
* Lasse Christensen
* David Bjørklund

for the DTU Course "Netværkspraktik"
#Classes
Below all classes and methods are explained
##FTPClient
FTPClient(string server_ip, uint16_t port, string user, string password, bool passive_mode);

###uint16_t get_port_number(string msg_227)
Method for translating a telnet 227 response to a port-number

###bool create_socket()
Method for creating a new socket contained within the class

###bool open_connection()
Method for opening the socket-connection. create_socket() must have been called before this method

###bool send_cmd(string message)
Method for sending command to the server

###string receive_response()
Method for receiving response from previously sent command

##DataReceiveSocket
This class is intended to be used for opening a socket to receive data through.
This class does not allow sending commands through socket, but only down- and uploading files

###DataSocket(string server_ip, uint16_t port)
Constructor method takes an IP (should be the same IP as ftp-sockekt is connected to)
and a port number

###~DataSocket(void)
Destructor method will close socket.

###bool create_socket()
Method for creating a new socket contained within the class

##bool open_connection()
Method for opening the socket-connection. create_socket() must have been called before this method

###void receive_file(string file_name)
Method for downlading a file to the Desktop

###void send_file(string file_name, string upload_path)
Method for uploading a file from the Desktop folder

###void close_socket()
Method will close the socket

##Helper

###static void raiseError(string message)
Method will print a message and exit application with exitcode 1.
###static void print_message(string message)
Method to print messages to the terminal
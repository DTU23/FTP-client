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

###bool send_cmd(string message);
Method for sending command to the server

###string receive_response();

##DataReceiveSocket

##Helper
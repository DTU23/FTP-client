#include "modules/FTPClient.h"
#include "modules/DataSocket.h"
#include "modules/FilePrinter.h"
#include <cassert>

#define SERVER_IP "130.226.195.126"
#define FTP_PORT 21

void print_menu();

int main( int argc, char** argv) {
    // Useless ASCII-art welcome message printed
    Helper::print_message("                                                               ______ _________         \n"
                                  "                                                              (  __  \\\\__   __/|\\     /|\n"
                                  "                                                              | (  \\  )  ) (   | )   ( |\n"
                                  "                                                              | |   ) |  | |   | |   | |\n"
                                  "                                                              | |   | |  | |   | |   | |\n"
                                  "                                                              | |   ) |  | |   | |   | |\n"
                                  "                                                              | (__/  )  | |   | (___) |\n"
                                  "                                                              (______/   )_(   (_______)\n"
                                  "                                                                                        \n"
                                  "    _        _______ _________          _______  _______  _______  _        _______  _______  _______  _______  _       __________________ _       \n"
                                  "   ( (    /|(  ____ \\\\__   __/|\\     /|(  ___  )(  ____ \\(  ____ )| \\    /\\(  ____ \\(  ____ )(  ____ )(  ___  )| \\    /\\\\__   __/\\__   __/| \\    /\\\n"
                                  "   |  \\  ( || (    \\/   ) (   | )   ( || (   ) || (    \\/| (    )||  \\  / /| (    \\/| (    )|| (    )|| (   ) ||  \\  / /   ) (      ) (   |  \\  / /\n"
                                  "   |   \\ | || (__       | |   | |   | || (___) || (__    | (____)||  (_/ / | (_____ | (____)|| (____)|| (___) ||  (_/ /    | |      | |   |  (_/ / \n"
                                  "   | (\\ \\) ||  __)      | |   ( (   ) )|  ___  ||  __)   |     __)|   _ (  (_____  )|  _____)|     __)|  ___  ||   _ (     | |      | |   |   _ (  \n"
                                  "   | | \\   || (         | |    \\ \\_/ / | (   ) || (      | (\\ (   |  ( \\ \\       ) || (      | (\\ (   | (   ) ||  ( \\ \\    | |      | |   |  ( \\ \\ \n"
                                  "   | )  \\  || (____/\\   | |     \\   /  | )   ( || (____/\\| ) \\ \\__|  /  \\ \\/\\____) || )      | ) \\ \\__| )   ( ||  /  \\ \\   | |   ___) (___|  /  \\ \\\n"
                                  "   |/    )_)(_______/   )_(      \\_/   |/     \\|(_______/|/   \\__/|_/    \\/\\_______)|/       |/   \\__/|/     \\||_/    \\/   )_(   \\_______/|_/    \\/\n"
                                  "                                                                                                                                                   ");

    // Instantiate FTPClient as anonymous user using blank password
    FTPClient ftpClient(SERVER_IP, FTP_PORT, "anonymous", "");

    // Flag for stopping loop
    bool run = true;
    do{

        // buffer for receiving menu-choice
        char menu_choice[256];
        // Print menu before asking for a new input
        print_menu();
        // assert used to avoid NULL arithmatic
        assert(fgets(menu_choice, 256, stdin) != NULL);
        // Detect enter-button
        if(menu_choice[strlen(menu_choice) - 1] == '\n') {
            // changing last \n to \0 to remove line break
            menu_choice[strlen(menu_choice) - 1] = '\0';
            // Treat menu choice as integer
            switch (atoi(menu_choice)){
                /**
                 * Choice 1 will download the small file.txt from the root-folder
                 */
                case 1:{
                    // Enter passive mod on FTP-socket
                    ftpClient.enter_passive_mode();
                    // Open datasocket on FTP data-port
                    DataSocket DataSocket(SERVER_IP, ftpClient.get_data_port_number());
                    // Send retrieve command on FTPClient
                    if (!ftpClient.send_cmd("RETR file.txt\r\n")) {
                        Helper::raiseError("Error receiving file.txt");
                    }
                    Helper::print_message(ftpClient.get_response());
                    Helper::print_message("File contents: ");
                    // Receive file from socket and print contents from filepath returned
                    printFileCont(DataSocket.receive_file("file.txt"));
                    // Print ftp-client response
                    Helper::print_message(ftpClient.get_response());
                    break;
                }
                /**
                 * Choice 2 will download the larger pdf-file from the /pub/62501/ folder
                 */
                case 2:{
                    // Enter passive mod on FTP-socket
                    ftpClient.enter_passive_mode();
                    // Open datasocket on FTP data-port
                    DataSocket DataSocket(SERVER_IP, ftpClient.get_data_port_number());
                    // Send retrieve command on FTPClient
                    if (!ftpClient.send_cmd("RETR /pub/62501/linuxchecklist.pdf\r\n")) {
                        Helper::raiseError("Error receiving file.txt");
                    }
                    Helper::print_message(ftpClient.get_response());
                    // Receive file from socket.
                    Helper::print_message("File contents: ");
                    // Receive file from socket and print contents from filepath returned
                    printFileCont(DataSocket.receive_file("linuxchecklist.pdf"));
                    // Print ftp-client response
                    Helper::print_message(ftpClient.get_response());
                    break;
                }
                /**
                 * Choice 3 will attempt uploading a file
                 */
                case 3:{
                    // Enter passive mod on FTP-socket
                    ftpClient.enter_passive_mode();
                    // Open datasocket on FTP data-port
                    DataSocket DataSocket(SERVER_IP, ftpClient.get_data_port_number());
                    // Send STOR-command on FTPClient
                    if (!ftpClient.send_cmd("STOR upload.txt\r\n")) {
                        Helper::raiseError("Error storing file.txt");
                    }
                    // Print response
                    DataSocket.send_file("send.txt");
                    Helper::print_message(ftpClient.get_response());
                    break;
                }
                /**
                 * Choice 4 will stop the loop and control sequence will come to an end
                 */
                case 4:
                    run = false;
                    break;
                /**
                 * Defaul will catch all non-valid menu-choices
                 */
                default:
                    Helper::print_message("invalid choice!");
                    break;
            }
        }
    }while(run);
}

/**
 * Method to print our ftp-client-menu
 */
void print_menu(){
    Helper::print_message(" ___  ___  ___   ___  _  _             _   \n"
                                  "| __>|_ _|| . \\ |  _>| |<_> ___ ._ _ _| |_ \n"
                                  "| _>  | | |  _/ | <__| || |/ ._>| ' | | |  \n"
                                  "|_|   |_| |_|   `___/|_||_|\\___.|_|_| |_|  \n"
                                  "                                           ");
    Helper::print_message("\t1. Download file.txt (11 Byte)");
    Helper::print_message("\t2. Download linuxchecklist.pdf (268,4KB)");
    Helper::print_message("\t3. Upload file");
    Helper::print_message("\t4. Exit");
}
#include "modules/FTPClient.h"
#include "modules/DataSocket.h"
#include <cassert>

#define SERVER_IP "130.226.195.126"
#define FTP_PORT 21

int main( int argc, char** argv) {
    // Useless ASCII-art welcome message printed
    Helper::print_message(" ____    ______  __  __      ____                                                   ___       __     \n"
                                  "/\\  _`\\ /\\__  _\\/\\ \\/\\ \\    /\\  _`\\                                               /'___`\\   /'__`\\   \n"
                                  "\\ \\ \\/\\ \\/_/\\ \\/\\ \\ \\ \\ \\   \\ \\ \\L\\_\\  _ __   __  __  _____   _____      __      /\\_\\ /\\ \\ /\\_\\L\\ \\  \n"
                                  " \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\ \\   \\ \\ \\L_L /\\`'__\\/\\ \\/\\ \\/\\ '__`\\/\\ '__`\\  /'__`\\    \\/_/// /__\\/_/_\\_<_ \n"
                                  "  \\ \\ \\_\\ \\ \\ \\ \\ \\ \\ \\_\\ \\   \\ \\ \\/, \\ \\ \\/ \\ \\ \\_\\ \\ \\ \\L\\ \\ \\ \\L\\ \\/\\  __/       // /_\\ \\ /\\ \\L\\ \\\n"
                                  "   \\ \\____/  \\ \\_\\ \\ \\_____\\   \\ \\____/\\ \\_\\  \\ \\____/\\ \\ ,__/\\ \\ ,__/\\ \\____\\     /\\______/ \\ \\____/\n"
                                  "    \\/___/    \\/_/  \\/_____/    \\/___/  \\/_/   \\/___/  \\ \\ \\/  \\ \\ \\/  \\/____/     \\/_____/   \\/___/ \n"
                                  "                                                        \\ \\_\\   \\ \\_\\                                \n"
                                  "                                                         \\/_/    \\/_/                                ");
    // Instantiate FTPClient as anonymous user in passive mode
    FTPClient ftpClient(SERVER_IP, FTP_PORT, "anonymous", "", true);
    // Get data transfer port from FTPClient
    uint16_t port = ftpClient.get_data_port_number();

    /**
     * Open new DataSocket for FTP file transfer on data-transfer port
     */
    DataSocket DataSocket(SERVER_IP, port);

    bool run = true;
    char menu_choice[256];
    Helper::print_message(" ___  ___  ___   ___  _  _             _   \n"
                                  "| __>|_ _|| . \\ |  _>| |<_> ___ ._ _ _| |_ \n"
                                  "| _>  | | |  _/ | <__| || |/ ._>| ' | | |  \n"
                                  "|_|   |_| |_|   `___/|_||_|\\___.|_|_| |_|  \n"
                                  "                                           ");
    Helper::print_message("\t1. Download file.txt");
    Helper::print_message("\t2. Download large file");
    Helper::print_message("\t3. Upload file");
    Helper::print_message("\t4. Print file contents");
    Helper::print_message("\t5. Exit");
    while(run)
    {
        assert(fgets(menu_choice, 256, stdin) != NULL);
        // Verifies that the previous character to the last character in the
        // buffer array is '\n' (The last character is '\0') if the
        // character is '\n' leaves loop.
        if(menu_choice[strlen(menu_choice) - 1] == '\n')
        {
            // fgets reads and adds '\n' in the string, replace '\n' by '\0' to
            // remove the line break .
            menu_choice[strlen(menu_choice) - 1] = '\0';
            switch (atoi(menu_choice)){
                case 1:
                    // Send retrieve command on FTPClient
                    if (!ftpClient.send_cmd("RETR file.txt\r\n")) {
                        Helper::raiseError("Error receiving file.txt");
                    }
                    // Receive file from socket.
                    DataSocket.receive_file("file.txt");
                    Helper::print_message("File retrieved");
                    break;
                case 2:
                    Helper::print_message("you chose 2");
                    break;
                case 3:
                    // Send store command on FTPClient
                    if (!ftpClient.send_cmd("STOR upload.txt\r\n")) {
                        Helper::raiseError("Error storing file.txt");
                    }
                    DataSocket.send_file("send.txt");
                    Helper::print_message("File sent to FTP server!");
                    break;
                case 4:
                    Helper::print_message("you chose 4");
                    break;
                case 5:
                    exit(0);
                default:
                    Helper::print_message("invalid choice!");
                    break;
            }
        }
    }
    return 0;
}
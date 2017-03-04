#include "FilePrinter.h"

int printFileCont (string filePath) {
    char buffer[BUFFER_SIZE];
    char *bufferPtr = buffer;
    ifstream file(filePath.c_str(), ios::binary);

    if (file.is_open()) {
        // Copy buffer_size of data to memory
        file.read(bufferPtr, sizeof(buffer));
        for (int i = 0; i < BUFFER_SIZE; i++) {
            if (int(buffer[i]) < 0) {
                cout << hex << setw(2) << setfill('0') << int(buffer[i]) + 256;
            } else {
                cout << hex << setw(2) << setfill('0') << int(buffer[i]);
            } // end else
            cout << " ";

            //Insert linebreaks for every 16 bytes
            if ((i + 1) % 16 == 0) {
                cout << endl;
            }
        } // end for
    } // end if(file.is_open)
    else{
        cout << "File not found" << endl;
        return -1;
    }
    file.close();
    return 0;
} // end main
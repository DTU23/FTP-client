#ifndef FILEPRINTER_H
#define FILEPRINTER_H

//Include input and output from screen
#include <iostream>
// Manipulators for streams
#include <iomanip>
// Include filestreams
#include <fstream>
#include <cstdint>
#include <string>

#define BUFFER_SIZE 1024 // has to be a multiple of two!

using namespace std;

int printFileCont (string filePath);

#endif //FILEPRINTER_H

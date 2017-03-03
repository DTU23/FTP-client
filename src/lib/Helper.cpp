#include "Helper.h"

/**
 * General method for printing messages to the console
 * @param message
 */
void Helper::print_message(string message) {
    cout << message << endl;
}

/**
 * Method will print an error message and exit program
 * @param message
 */
void Helper::raiseError(string message) {
    Helper::print_message(message);
    exit(1);
}
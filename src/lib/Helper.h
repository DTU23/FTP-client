#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <string.h>
#include <stdio.h>
#include<iostream>

using namespace std;

class Helper {
public:
    static void raiseError(string message);
    static void print_message(string message);
};

#endif //HELPER_H
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <exception>
#include <cerrno>
#include <cstring>
#include "RBTree.h"
#include "customError.h"
#pragma warning(disable:4996)

using namespace std;

bool inputValidation(int argc, char* argv[]) {
    bool retValue = false;
    struct stat fileInfo;
    string errorString;
    int error;

    if (1 >= argc || 3 <= argc) {
        cout << "USAGE: " << argv[0] << " [FILENAME]" << endl;
        cout << "exiting..." << endl;
    }
    else {
        error = stat(argv[1], &fileInfo);

        if (0 != error) {
            errorString = strerror(errno);
            errorString += ": ";
            errorString += argv[1];
            throw runtime_error(errorString);
        }

        if (0 == fileInfo.st_size) {
            errorString = "file is empty: ";
            errorString += argv[1];
            throw runtime_error(errorString);
        }

        retValue = true;
    }

    return retValue;
};



int main(int argc, char* argv[])
{
    bool retValue = false;

    try {
        retValue = inputValidation(argc, argv);
    }
    catch (runtime_error& e) {
        cout << "RUNTIME_ERROR: " << e.what() << endl;
    }
    catch (exception& e) {
        cout << "EXCEPTION: " << e.what() << endl;
    }

        if (retValue) {
            ifstream myFile;
            C_RBTree numTree;
            int currentCommand;
            int currentValue;

            myFile.open(argv[1]);
            if (myFile.is_open()) {
                try {
                    while (!myFile.eof()) {
                        myFile >> currentCommand;
                        switch (currentCommand) {
                        case 0:
                            myFile >> currentValue;
                            cout << "Attempting to insert " << currentValue << "..." << endl;
                            numTree.P_insertValue(currentValue);
                            break;
                        case 1:
                            myFile >> currentValue;
                            cout << "Attempting to delete " << currentValue << "..." << endl;
                            numTree.P_deleteNode(currentValue);
                            break;
                        case 2:
                            cout << "Printing current tree..." << endl;
                            numTree.P_printTree();
                            break;
                        case 8:
                            cout << "Attempting to delete current tree..." << endl;
                            numTree.P_deleteTree();
                            break;
                        case 9:
                            cout << "Exiting program..." << endl;
                            numTree.P_deleteTree();
                            exit(1);
                            break;
                        default:
                            cout << "WARNING: " << currentCommand << " is NOT a valid command. Please delete or modify " << currentCommand << " to be compliant to scheme and try again" << endl;
                            cout << "ALLOWABLE COMMANDS:" << endl << "0 -> insertion" << endl << "1 -> deletion" << endl << "2 -> print tree" << endl << "8 -> delete tree" << endl << "9 -> endProgram" << endl;
                            exit(1);
                        }
                    }
                }
                catch (MyException e) {
                    cout << e.what() << endl;
                }
                catch (exception& e) {
                    cout << "EXCEPTION: " << e.what() << endl;
                }
            }
    }
    
}

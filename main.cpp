#include <iostream>
#include "enigma.h"
#include "errors.h"
using namespace std;

int const MAX_LENGTH = 100;
int const MIN_PARAMETERS = 4; // 

int prompt_for_input (char& input);
void check_error (int res);

int main(int argc, char** argv) {
    int res;
    // if (argc < MIN_PARAMETERS) 
    //  res = INSUFFICIENT_NUMBER_OF_PARAMETERS;
    // check_error(res);

    // configure settings
    char * pb_file = argv[1];
    Plugboard pb(pb_file);
    res = pb.setup();
    check_error(res);

    char * rf_file = argv[2];
    Reflector rf(rf_file);
    res = rf.setup();
    check_error(res);

    char input;
    cout << "This program simulates a general Enigma machine.\n";
    
    res = prompt_for_input(input);
    check_error(res);
    if (pb.num) 
        pb.process_input(input);
    rf.process_input(input);

    cout << input << endl;
    return 0;
}

int prompt_for_input (char& input) {
    cout << "Please input an uppercase letter (A-Z) to be encrypted/decrypted.\n";
    // receive an array of chars and read in a loop
    // check for invalid chars on looping
    // discard whitespac
    // char input_arr [MAX_LENGTH];
    // cin >> input_arr;

    // for (int i=0; i<MAX_LENGTH; i++) {
    //     input = input_arr[i];

    // }

    // cout << input_arr << endl;
        
    cin >> input;

    if (input < 'A' || input > 'Z') 
        return INVALID_INPUT_CHARACTER;

    return NO_ERROR;

}

void check_error (int res) {
    if (res == 0)
        return;
    else
        cout << "Error. ";
     
    switch (res) {
        case INSUFFICIENT_NUMBER_OF_PARAMETERS:
            cout << "Insufficient number of parameters.\n";
            break;
        case INVALID_INPUT_CHARACTER: 
            cout << "Invalid input character.\n";
            break;
        case INVALID_INDEX:
            cout << "Invalid index.\n";
            break;
        case NON_NUMERIC_CHARACTER:
            cout << "Non-numeric character.\n";
            break;
        case IMPOSSIBLE_PLUGBOARD_CONFIGURATION:
            cout << "Impossible plugboard configuration.\n";
            break;
        case INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS:
            cout << "Incorrect number of plugboard parameters.\n";
            break;
        case INVALID_ROTOR_MAPPING:
            cout << "Invalid rotor mapping.\n";
            break;
        case NO_ROTOR_STARTING_POSITION:
            cout << "No rotor starting position.\n";
            break;
        case INVALID_REFLECTOR_MAPPING:
            cout << "Invalid reflector mapping.\n";
            break;
        case INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS:
            cout << "Incorrect number of reflector parameters.\n";
            break;
        case ERROR_OPENING_CONFIGURATION_FILE:
            cout << "Error opening configuration file.\n";
            break;
        default:
            cout << "\n";
    }
    exit(1);
    return;
    // exit?
}
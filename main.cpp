#include <iostream>
#include "enigma.h"
#include "errors.h"
using namespace std;

int main(int argc, char** argv) {
    char input[MAX_LENGTH], output[MAX_LENGTH];
    int res = 0, num_of_rotors = 0, output_length = 0;
    if (argc < MIN_PARAMETERS) {
        cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>)* rotor-postions\n";
        res = INSUFFICIENT_NUMBER_OF_PARAMETERS;
    }
    check_error(res);

    num_of_rotors = argc - MIN_PARAMETERS;

    // configure settings
    char * pb_file = argv[1];
    Plugboard pb(pb_file);
    res = pb.setup();
    check_error(res);

    char * rf_file = argv[2];
    Reflector rf(rf_file);
    res = rf.setup();
    check_error(res);

    char * pos_file = argv[argc-1];
    int starting_pos[num_of_rotors];
    res = open_pos_file(pos_file, num_of_rotors, starting_pos);
    check_error(res);

    Rotor** rotors_ptr = setup_rotors(num_of_rotors, argv, starting_pos);

    cin.getline(input, MAX_LENGTH);
    char error_input;
    res = process_inputs(input, output, output_length, num_of_rotors, pb, rotors_ptr, rf, error_input);
    output[output_length] = '\0';

    for (int i=0; output[i] != '\0' && i<output_length; i++)
      cout << output[i];

    if (res == INVALID_INPUT_CHARACTER) {
        cerr << error_input << " is not a valid input character "
            << "(input characters must be upper case letters A-Z)!\n";
    }

    check_error(res);
    
    for (int i=0; i<num_of_rotors; i++)
        delete rotors_ptr[i];
    delete [] rotors_ptr;

    return NO_ERROR;
}


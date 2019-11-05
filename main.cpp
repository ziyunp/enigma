#include <iostream>
#include "enigma.h"
#include "errors.h"
using namespace std;

int main(int argc, char** argv) {
    char input[MAX_LENGTH];
    int res = 0, num_of_rotors = 0, input_length = 0;
    if (argc < MIN_PARAMETERS) {
        res = INSUFFICIENT_NUMBER_OF_PARAMETERS;
        check_error(res, "command line");
    }
    num_of_rotors = argc - MIN_PARAMETERS;

    // configure settings
    char * pb_file = argv[1];
    Plugboard pb(pb_file);
    res = pb.setup();
    check_error(res, "plugboard");

    char * rf_file = argv[2];
    Reflector rf(rf_file);
    res = rf.setup();
    check_error(res, "reflector");

    char * pos_file = argv[argc-1];
    int starting_pos[num_of_rotors];
    res = open_pos_file(pos_file, num_of_rotors, starting_pos);
    check_error(res, "rotor positions");

    Rotor** rotors_ptr = setup_rotors(num_of_rotors, argv, starting_pos); // need cleaning

    res = prompt_for_input(input, input_length);
    check_error(res, "input");
    char output[input_length + 1];
    // process input: pb -> rotors -> rf -> rotors(backwards) -> pb
    int i;
    for (i=0; i<input_length; i++) {
        int letter = input[i] - 'A';
        pb.process_input(letter);

        bool mapped_backwards = false;
        if (num_of_rotors > 0) 
            rotors_processing(letter, num_of_rotors, rotors_ptr, mapped_backwards);
        rf.process_input(letter); 
        mapped_backwards = true;
        if (num_of_rotors > 0)
            rotors_processing(letter, num_of_rotors, rotors_ptr, mapped_backwards);

        pb.process_input(letter);

        output[i] = letter + 'A';
    }
    output[input_length] = '\0';
    cout << output << endl;

    for (int i=0; i<num_of_rotors; i++)
        delete rotors_ptr[i];
    delete [] rotors_ptr;
    return 0;
}


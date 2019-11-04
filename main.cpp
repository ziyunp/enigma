#include <iostream>
#include "enigma.h"
#include "errors.h"
using namespace std;

int main(int argc, char** argv) {
    char input[MAX_LENGTH], output[MAX_LENGTH];
    int res = 0, num_of_rotors = 0, input_length = 0;
    if (argc < MIN_PARAMETERS) {
        res = INSUFFICIENT_NUMBER_OF_PARAMETERS;
        check_error(res);
    }
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

    Rotor** rotors_ptr = setup_rotors(num_of_rotors, argv, starting_pos); // need cleaning

    cout << "This program simulates a general Enigma machine.\n";
    res = prompt_for_input(input, input_length);
    check_error(res);
    for (int i=0; i<input_length; i++) 
        cout << input[i] << endl;
        
    // process input: pb -> rotors -> rf -> rotors(backwards) -> pb
    for (int i=0; i<input_length; i++) {
        int letter = input[i] - 'A';
        pb.process_input(letter);

        bool mapped_backwards = false;
        rotors_processing(letter, num_of_rotors, rotors_ptr, mapped_backwards);
        rf.process_input(letter); 
        mapped_backwards = true;
        rotors_processing(letter, num_of_rotors, rotors_ptr, mapped_backwards);

        pb.process_input(letter);

        output[i] = letter + 'A';
    }
    for (int i=0; i<input_length; i++) 
        cout << output[i] << endl;

    return 0;
}


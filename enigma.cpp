#include <iostream>
#include <fstream>

#include "enigma.h"
#include "errors.h"
using namespace std;

Plugboard::Plugboard (char * config) : config_file(config) {}

int Plugboard::setup() {
  ifstream in(config_file);
  if (!in) {
    // fetch file name rather than hard coding!!
    cerr << "Error opening plugboard file plugboard.pb\n";
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  
  int count, next_ch;
  for (count=0; !in.eof() && !in.fail(); count++) {
    in >> ws;
    next_ch = in.peek();
    
    if (next_ch != char_traits<char>::eof() && !isdigit(next_ch)) {
      cerr << "Non-numeric character in plugboard file plugboard.pb\n";
      return NON_NUMERIC_CHARACTER;
    }

    in >> pb_config[count] >> ws;
  }

  if (in.fail()) {
    if (next_ch == char_traits<char>::eof())
      count = 0;
    else {
      cerr << "Error reading plugboard file plugboard.pb\n";
      return ERROR_OPENING_CONFIGURATION_FILE;
    }
  }
  
  num = count;
  if (num > 0) {
    if(num % 2 != 0) {
      cerr << "Incorrect number of parameters in plugboard file plugboard.pb\n";
      return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    }

    for (int i=0; i < num; i++) {
      if (pb_config[i] < 0 || pb_config[i] > 25){
        cerr << "Invalid index in plugboard file plugboard.pb (number should be between 0-25)\n";
        return INVALID_INDEX;
      }
      
      for (int j= i + 1; j < num; j++) {
        if(pb_config[i] == pb_config[j]) {
          cerr << "Impossible plugboard configuration. There is more than one attempt to make contact with " << pb_config[i] << endl;
          return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
        }
      }
    }
  }
  return NO_ERROR;
}

void Plugboard::process_input(int& input) {
  if (num == 0) return;
  for (int i=0; i < num; i++) {
    if (pb_config[i] == input) {
      if (i % 2 == 0)
        input = pb_config[i+1];
      else
        input = pb_config[i-1];
      return;
    }
  }
}

Reflector::Reflector (char * config) : config_file(config) {}

int Reflector::setup() {
  ifstream in(config_file);
  if (!in) {
    cerr << "Error opening reflector file reflector.rf\n";
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  int count, next_ch;
  for (count=0; !in.eof() && !in.fail(); count++) {
    in >> ws;
    next_ch = in.peek();

    if (!isdigit(next_ch)) {
      cerr << "Non-numeric character in reflector file reflector.rf\n";
      return NON_NUMERIC_CHARACTER;
    }

    in >> rf_config[count] >> ws;
  }

  if (in.fail()) {
    cerr << "Error reading reflector file reflector.rf\n";
    return ERROR_OPENING_CONFIGURATION_FILE;

  }
  
  int num = count;
  if (num % 2) {
    cerr << "Incorrect (odd) number of parameters in reflector file reflector.rf\n";
    return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
  } else {
    if(num < TOTAL_ALPHABET_COUNT) {
      cerr << "Insufficient number of mappings in reflector file: reflector.rf\n";
      return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    } else if (num > TOTAL_ALPHABET_COUNT) {
      cerr << "Too many number of mappings in reflector file reflector.rf\n";
      return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    }
  }

  for (int i=0; i < TOTAL_ALPHABET_COUNT; i++) {
    if (rf_config[i] < 0 || rf_config[i] > 25) {
      cerr << "Invalid index in reflector file reflector.rf (number should be between 0-25)\n";
      return INVALID_INDEX;
    }
    
    for (int j= i + 1; j < TOTAL_ALPHABET_COUNT; j++) {
      if(rf_config[i] == rf_config[j]) {
        cerr << "Invalid reflector mapping: duplicated mapping of " 
          << rf_config[i] << endl;
        return INVALID_REFLECTOR_MAPPING;
      }
    }
  }
  return NO_ERROR;
}

void Reflector::process_input(int& input) {
  for (int i=0; i < TOTAL_ALPHABET_COUNT; i++) {
    if (rf_config[i] == input) {
      if (i % 2 == 0)
        input = rf_config[i+1];
      else
        input = rf_config[i-1];
      return;
    }
  }
}

Rotor::Rotor (char * config) : config_file(config) {}

int Rotor::setup() {
  ifstream in(config_file);
  if (!in) {
    cerr << "Error opening rotor file rotor.rot\n";
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  
  int notch_index = 0;
  int count, num_of_notch, next_ch;
  
  for (count=0; !in.eof() && !in.fail(); count++) {
    in >> ws;
    next_ch = in.peek();

    if (!isdigit(next_ch)) {
      cerr << "Non-numeric character for mapping in rotor file rotor.rot\n";
      return NON_NUMERIC_CHARACTER;
    }

    if (count >= TOTAL_ALPHABET_COUNT)
      in >> notch[notch_index++] >> ws;
    else 
      in >> rot_config[count] >> ws;
    }

  num_of_notch = notch_index;

   if (in.fail()) {
    cerr << "Error reading rotor file rotor.rot\n";
    return ERROR_OPENING_CONFIGURATION_FILE;
  }

  if(count < TOTAL_ALPHABET_COUNT) {
    cerr << "Not all inputs mapped in rotor file: rotor.rot\n";
    return INVALID_ROTOR_MAPPING;
  }

  for (int i=0; i < TOTAL_ALPHABET_COUNT; i++) {
    if (rot_config[i] < 0 || rot_config[i] > 25) {
      cerr << "Invalid index in rotor configuration (number should be between 0-25)\n";
      return INVALID_INDEX;
    }
    
    for (int j= i + 1; j < TOTAL_ALPHABET_COUNT; j++) {
      if(rot_config[i] == rot_config[j]) {
        // end with 'in'?
        cerr << "Invalid mapping of input " << j << " to output " 
          << rot_config[j] << " (output " << rot_config[i] 
          << " is already mapped to from input " << i << ") in\n";
        return INVALID_ROTOR_MAPPING;
      }
    }
  }

  // check for repeated notch value
  for (int i=0; i < num_of_notch; i++) {
    if (notch[i] < 0 || notch[i] > 25) {
      cerr << "Invalid index for turnover notches (number should be between 0-25)\n";
      return INVALID_INDEX;
    }
    
    for (int j= i + 1; j < num_of_notch; j++) {
      if(notch[i] == notch[j]) { 
        cerr << "Invalid mapping of notches: duplicated mapping of " << notch[i] << endl;
        return INVALID_ROTOR_MAPPING;
      }
    }
  }
  return NO_ERROR;
}

void Rotor::set_starting_position(int init) {
  if(rot_config[0] == init) 
    return;

  rotate();
  return set_starting_position(init);
}

bool Rotor::process_input(int& input, bool rotate_self, bool mapped_backwards) {
  bool notch_triggered = false;
  if (rotate_self) {
    rotate();
    notch_triggered = rotate_next(); 
  }

  if (mapped_backwards) {
    int target;
    for (target = 0; target < TOTAL_ALPHABET_COUNT; target++) {
      if (input == rot_config[target]) {
        input = target;
        break;
      }
    }
  } else
      input = rot_config[input];

  return notch_triggered;
}

void Rotor::rotate() {
  int first = rot_config[0];
  int index = 0;
  for (; index < TOTAL_ALPHABET_COUNT; index++) {
    if (index == TOTAL_ALPHABET_COUNT - 1)
      rot_config[index] = first;
    else rot_config[index] = rot_config[index + 1];
  }
}

bool Rotor::rotate_next() {
  for (int i=0; i<num_of_notch; i++) {
    // definition of the notch ???
    if (rot_config[0] == notch[i]) {
      return true;
      }
  }
  return false;
}

Rotor** setup_rotors(int num, char** const argv, int const starting_pos[]) {
    if (num == 0) return NULL;

    Rotor** rot_ptr = new Rotor * [num] {};
    int const min_file_index = 3, max_file_index = 3 + num;
    int file_index = min_file_index, i=0;

    for (; file_index < max_file_index; file_index++) {
        char * rot_file = argv[file_index];
        Rotor* rotor = new Rotor(rot_file);
        int res = rotor->setup();
        check_error(res);
        rotor->set_starting_position(starting_pos[i]);
        rot_ptr[i] = rotor;
        i++;
    }
    return rot_ptr;
} 

int open_pos_file(char * pos_file, int num_of_rotors, int starting_pos[]) {
    ifstream in(pos_file);
    if (!in) {
      cerr << "Error opening rotor positions file rotor.pos\n";
      return ERROR_OPENING_CONFIGURATION_FILE;
    }

    int count, next_ch;
    for (count=0; count < num_of_rotors && !in.eof() && !in.fail(); count++) {
      in >> ws;
      next_ch = in.peek();
      
      if (next_ch != char_traits<char>::eof() && !isdigit(next_ch)) {
        cerr << "Non-numeric character in rotor positions file rotor.pos\n";
        return NON_NUMERIC_CHARACTER;
      }

        in >> starting_pos[count] >> ws;
    }

    if (in.fail()) {
      cerr << "Error reading rotor positions file rotor.pos\n";
      return ERROR_OPENING_CONFIGURATION_FILE;
    }

    if(count < num_of_rotors) {
      if (num_of_rotors - count == 1)
        cerr << "No starting position for rotor 0 in rotor position file: rotor.pos\n";
      
      else
        cerr << "No starting position for more than 1 rotors in rotor position file: rotor.pos\n";

      return NO_ROTOR_STARTING_POSITION;
    }

    for (int i=0; i < num_of_rotors; i++) {
      if (starting_pos[i] < 0 || starting_pos[i] > 25) {
        cerr << "Invalid index in rotor position file: rotor.pos (number should be between 0-25)\n";
        return INVALID_INDEX;
      }
    }
    return NO_ERROR;
}

void rotors_processing(int& letter, int const num_of_rotors, Rotor** rotors_ptr, bool mapped_backwards){
    bool rotate_self = false, rotate_next = false;
    if (!mapped_backwards) {
        for (int i = num_of_rotors - 1; i >= 0; i--) {
            if (i == num_of_rotors - 1)
                rotate_self = true;
            else rotate_self = rotate_next;
            rotate_next = rotors_ptr[i]->process_input(letter, rotate_self, mapped_backwards);
        }
    } else {
        rotate_self = false;
        for (int i=0; i<num_of_rotors; i++) 
            rotors_ptr[i]->process_input(letter, rotate_self, mapped_backwards);
    }
}

int process_inputs(char const input[], char output[], int& output_length, int num_of_rotors, Plugboard pb, Rotor** rotors_ptr, Reflector rf, char& error_input) {
    int i;
    for (i=0; input[i] != '\0' && i < MAX_LENGTH; i++) {
        if (input[i] == ' ')
            continue;
        if (input[i] < 'A' || input[i] > 'Z') {
          error_input = input[i];
          return INVALID_INPUT_CHARACTER;
        }
  
        // process input: pb -> rotors -> rf -> rotors(backwards) -> pb
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

        output[output_length++] = letter + 'A';
    }

    return NO_ERROR;
}

void check_error (int res) {
    if (res == 0) 
        return;
    exit(res);
}

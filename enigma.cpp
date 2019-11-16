#include <iostream>
#include <fstream>
#include "enigma.h"
#include "errors.h"
using namespace std;

/**************************** Plugboard ****************************/

Plugboard::Plugboard (char * config) : config_file(config) {}

int Plugboard::setup() {
  ifstream in(config_file);
  if (!in) {
    cerr << "Error opening plugboard file " << config_file << endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  
  int next_ch;
  for (num_of_parameters=0; !in.eof() && !in.fail(); num_of_parameters++) {
    // eat up leading whitespace
    in >> ws; 
    next_ch = in.peek();
    
    if (next_ch != char_traits<char>::eof() && !isdigit(next_ch)) {
      cerr << "Non-numeric character in plugboard file " << config_file << endl;
      return NON_NUMERIC_CHARACTER;
    }
    // eat up whitespace after the integer
    in >> pb_config[num_of_parameters] >> ws; 
  }

  if (in.fail()) {
    if (next_ch == char_traits<char>::eof())
      num_of_parameters--;
    else {
      cerr << "Error reading plugboard file " << config_file << endl;
      return ERROR_OPENING_CONFIGURATION_FILE;
    }
  }
  
  // plugboard file can be empty i.e. num_of_parameters == 0
  if (num_of_parameters > 0) {
    // there must be an even number of parameters
    if(num_of_parameters % 2 != 0) {
      cerr << "Incorrect number of parameters in plugboard file " 
        << config_file << endl;
      return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;
    }

    for (int i=0; i < num_of_parameters; i++) {
      if (pb_config[i] < 0 || pb_config[i] > 25){
        cerr << "Invalid index in plugboard file " << config_file 
          << " (number should be between 0-25)\n";
        return INVALID_INDEX;
      }
      // check for duplicated parameters 
      // i.e. mapped to self or being mapped to for more than once
      for (int j= i + 1; j < num_of_parameters; j++) {
        if(pb_config[i] == pb_config[j]) {
          cerr << "Impossible plugboard configuration. There is more than one "   << "attempt to make contact with " << pb_config[i] << endl;
          return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
        }
      }
    }
  }
  in.close();
  return NO_ERROR;
}

void Plugboard::process_input(int& input) {
  if (num_of_parameters == 0) return;
  for (int i=0; i < num_of_parameters; i++) {
    if (pb_config[i] == input) {
      if (i % 2 == 0)
        input = pb_config[i+1];
      else
        input = pb_config[i-1];
      return;
    }
  }
}

/**************************** Reflector ****************************/

Reflector::Reflector (char * config) : config_file(config) {}

int Reflector::setup() {
  ifstream in(config_file);
  if (!in) {
    cerr << "Error opening reflector file " << config_file << endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  int count, next_ch;
  for (count=0; !in.eof() && !in.fail(); count++) {
    in >> ws;
    next_ch = in.peek();
  
    if (!isdigit(next_ch)) {
      cerr << "Non-numeric character in reflector file " << config_file << endl;
      return NON_NUMERIC_CHARACTER;
    }

    in >> rf_config[count] >> ws;
  }

  if (in.fail()) {
    cerr << "Error reading reflector file " << config_file << endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  
  // there must be exactly 13 pairs of parameters 
  if (count % 2) {
    cerr << "Incorrect (odd) number of parameters in reflector file " 
      << config_file << endl;
    return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
  } else {
    if(count < TOTAL_ALPHABET_COUNT) {
      cerr << "Insufficient number of mappings in reflector file: " 
        << config_file << endl;
      return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    } else if (count > TOTAL_ALPHABET_COUNT) {
      cerr << "Too many number of mappings in reflector file " << config_file     << endl;
      return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;
    }
  }

  for (int i=0; i < TOTAL_ALPHABET_COUNT; i++) {
    if (rf_config[i] < 0 || rf_config[i] > 25) {
      cerr << "Invalid index in reflector file " << config_file << " (number should be between 0-25)\n";
      return INVALID_INDEX;
    }
    // check for duplicated parameters 
    // i.e. mapped to self or being mapped to for more than once
    for (int j= i + 1; j < TOTAL_ALPHABET_COUNT; j++) {
      if(rf_config[i] == rf_config[j]) {
        cerr << "Invalid reflector mapping: duplicated mapping of " 
          << rf_config[i] << endl;
        return INVALID_REFLECTOR_MAPPING;
      }
    }
  }
  in.close();
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

/**************************** Rotor ****************************/

Rotor::Rotor (char * config) : config_file(config) {}

int Rotor::setup() {
  ifstream in(config_file);
  if (!in) {
    cerr << "Error opening rotor file " << config_file << endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }
  
  int count, next_ch;
  
  for (count=0; !in.eof() && !in.fail(); count++) {
    in >> ws;
    next_ch = in.peek();

    if (next_ch != char_traits<char>::eof() && !isdigit(next_ch)) {
      cerr << "Non-numeric character for mapping in rotor file " << config_file << endl;
      return NON_NUMERIC_CHARACTER;
    }

    // 0-25th parameters specify rotor configuration
    // 26th onwards specify the positions of notches
    if (count >= TOTAL_ALPHABET_COUNT)
      in >> notch[num_of_notch++] >> ws;
    else 
      in >> rot_config[count] >> ws;
  }
  
  if (in.fail()) {
    cerr << "Error reading rotor file " << config_file << endl; 
    return ERROR_OPENING_CONFIGURATION_FILE;
  }

  // there must be at least 26 parameters to specify rotor configuration
  if(count < TOTAL_ALPHABET_COUNT) {
    cerr << "Not all inputs mapped in rotor file: " << config_file << endl;
    return INVALID_ROTOR_MAPPING;
  }

  for (int i=0; i < TOTAL_ALPHABET_COUNT; i++) {
    if (rot_config[i] < 0 || rot_config[i] > 25) {
      cerr << "Invalid index in rotor configuration (number should be between 0-25)\n";
      return INVALID_INDEX;
    }
    
    // check for duplicated parameters 
    // i.e. mapped to self or being mapped to for more than once
    for (int j= i + 1; j < TOTAL_ALPHABET_COUNT; j++) {
      if(rot_config[i] == rot_config[j]) {
        cerr << "Invalid mapping of input " << j << " to output " 
          << rot_config[j] << " (output " << rot_config[i] 
          << " is already mapped to from input " << i << ") in rotor file: " << config_file << endl;
        return INVALID_ROTOR_MAPPING;
      }
    }
  }

  for (int i=0; i < num_of_notch; i++) {
    if (notch[i] < 0 || notch[i] > 25) {
      cerr << "Invalid index for turnover notches (number should be between 0-25)\n";
      return INVALID_INDEX;
    }
    // check for duplicated notch position
    for (int j= i + 1; j < num_of_notch; j++) {
      if(notch[i] == notch[j]) { 
        cerr << "Invalid mapping of notches: duplicated mapping of " << notch[i] << endl;
        return INVALID_ROTOR_MAPPING;
      }
    }
  }
  in.close();
  return NO_ERROR;
}

void Rotor::set_starting_position(int starting_pos) {
  if(starting_pos == 0) 
    return;
  else {
    // rotate for as many times as the specified starting_pos
    // after N rotations the Nth position will be at the top
    for (int i = starting_pos; i > 0; i--)
      rotate();
  }
}

bool Rotor::process_input(int& input, bool rotate_self, bool mapped_backwards) {
  bool notch_triggered = false;
  if (rotate_self)
    notch_triggered = rotate();

  if (mapped_backwards) {
    // + offset when mapped backwards
    int target = input + offset;
    if (target > TOTAL_ALPHABET_COUNT - 1) 
      target -= TOTAL_ALPHABET_COUNT;
  
    // output (modified input) as the index where target is matched
    for (int i=0; i<TOTAL_ALPHABET_COUNT; i++) {
      if (rot_config[i] == target)
        input = i;
    }
  } else {
    // - offset when mapped forwards
    // output (modified input) as the value at index == input
    input = rot_config[input] - offset;
    if (input < 0)
      input = TOTAL_ALPHABET_COUNT + input;
  }
  return notch_triggered;
}

bool Rotor::rotate() {
  if (offset < TOTAL_ALPHABET_COUNT - 1)
    offset++;
  else 
    offset = 0;

  int first = rot_config[0];
  for (int index = 0; index < TOTAL_ALPHABET_COUNT; index++) {
    if (index == TOTAL_ALPHABET_COUNT - 1)
      rot_config[index] = first;
    else rot_config[index] = rot_config[index + 1];
  }
  // check if a notch is triggered
  for (int i=0; i<num_of_notch; i++) {
    // offset indicates the number of rotations 
    // let's say a notch is located at the Nth position
    // after N rotations the notch will be at the top
    if (offset == notch[i])
      return true;
  }
  return false;
}

/**************************** Free Functions ****************************/

int get_starting_pos(char * pos_file, int num_of_rotors, int starting_pos[]) {
  ifstream in(pos_file);
  if (!in) {
    cerr << "Error opening rotor positions file " << pos_file << endl;
    return ERROR_OPENING_CONFIGURATION_FILE;
  }

  int count, next_ch;
  for (count=0; !in.eof() && !in.fail(); count++) {
    in >> ws;
    next_ch = in.peek();
    
    if (next_ch != char_traits<char>::eof() && !isdigit(next_ch)) {
      cerr << "Non-numeric character in rotor positions file " << pos_file << endl;
      return NON_NUMERIC_CHARACTER;
    }

    in >> starting_pos[count] >> ws;
  }

  if (in.fail()) {
    if (next_ch == char_traits<char>::eof())
      count--;
  }

  // check if there are sufficient starting positions for all rotors
  if(count < num_of_rotors) {
    if (num_of_rotors - count == 1) {
      cerr << "No starting position for rotor 0 in rotor position file: " << pos_file << endl;
    }
    else {
      cerr << "No starting position for more than 1 rotors in rotor position file: " << pos_file << endl;
    }
    return NO_ROTOR_STARTING_POSITION;
  }

  for (int i=0; i < num_of_rotors; i++) {
    if (starting_pos[i] < 0 || starting_pos[i] > 25) {
      cerr << "Invalid index in rotor position file: " << pos_file << " (number should be between 0-25)\n";
      return INVALID_INDEX;
    }
  }
  in.close();
  return NO_ERROR;
}

Rotor** setup_rotors(int num_of_rotors, char** const argv, int const starting_pos[]) {
  if (num_of_rotors == 0) return NULL;

  Rotor** rot_ptr = new Rotor * [num_of_rotors] {};
  int const min_file_index = 3, max_file_index = 3 + num_of_rotors;
  int file_index, i=0;

  for (file_index = min_file_index; file_index < max_file_index; file_index++) {
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

void rotors_processing(int& input, int const num_of_rotors, Rotor** rotors_ptr, bool mapped_backwards) {
  bool rotate_self = false, rotate_next = false;
  if (!mapped_backwards) {
    for (int i = num_of_rotors - 1; i >= 0; i--) {
      if (i == num_of_rotors - 1)
        rotate_self = true;
      else rotate_self = rotate_next;

      rotate_next = rotors_ptr[i]->process_input(input, rotate_self, mapped_backwards);
    }
  } else {
    rotate_self = false;
    for (int i=0; i<num_of_rotors; i++) 
      rotors_ptr[i]->process_input(input, rotate_self, mapped_backwards);
  }
}

int process_inputs(char const input[], char output[], int& output_length, int num_of_rotors, Plugboard pb, Rotor** rotors_ptr, Reflector rf, char& error_input) {
  for (int i=0; input[i] != '\0' && i < MAX_LENGTH; i++) {
    // ignore any whitespace
    if (input[i] == ' ')
        continue;

    if (input[i] < 'A' || input[i] > 'Z') {
      error_input = input[i];
      return INVALID_INPUT_CHARACTER;
    }

    int letter = input[i] - 'A';

    // first plugboard process: 
    pb.process_input(letter);

    // passing through rotors R-L (forwards): 
    bool mapped_backwards = false;
    if (num_of_rotors > 0) 
      rotors_processing(letter, num_of_rotors, rotors_ptr, mapped_backwards);

    // reflector process:
    rf.process_input(letter); 

    // passing through rotors L-R (backwards):
    mapped_backwards = true;
    if (num_of_rotors > 0)
      rotors_processing(letter, num_of_rotors, rotors_ptr, mapped_backwards);

    // second plugboard process: 
    pb.process_input(letter);

    output[output_length++] = letter + 'A';
  }
  return NO_ERROR;
}

void check_error (int res) {
  if (res == NO_ERROR) 
    return;
  exit(res);
}

#include <iostream>
#include <fstream>

#include "enigma.h"
#include "errors.h"
using namespace std;


Plugboard::Plugboard (char * config) : config_file(config) {}

int Plugboard::setup() {
  // assign values to data members: pb_config[], num

  ifstream in(config_file);
  if (!in) 
    return ERROR_OPENING_CONFIGURATION_FILE;

  int count = 0;
  in >> pb_config[count] >> ws;

  while(!in.eof() && !in.fail()) {
    in >> pb_config[++count] >> ws;
  }

  if(in.fail())
    return NON_NUMERIC_CHARACTER;
  
  if (count > 0) {
    num = count + 1;
    if(num % 2 != 0)
      return INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS;

    for (int i=0; i < num; i++) {
      if (pb_config[i] < 0 || pb_config[i] > 25)
        return INVALID_INDEX;
      
      for (int j= i + 1; j < num; j++) {
        if(pb_config[i] == pb_config[j]) 
          return IMPOSSIBLE_PLUGBOARD_CONFIGURATION;
      }
    }
  }
  return NO_ERROR;
}

void Plugboard::process_input(int& input) {
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
  // assign values to data members: rf_config[]

  ifstream in(config_file);
  if (!in) 
    return ERROR_OPENING_CONFIGURATION_FILE;

  int count = 0;
  in >> rf_config[count] >> ws;
    
  while(!in.eof() && !in.fail()) {
    in >> rf_config[++count] >> ws;
  }

  if(in.fail()) 
    return NON_NUMERIC_CHARACTER;

  int num = count + 1;
  if(num != TOTAL_ALPHABET_COUNT)
    return INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS;

  for (int i=0; i < TOTAL_ALPHABET_COUNT; i++) {
    if (rf_config[i] < 0 || rf_config[i] > 25)
      return INVALID_INDEX;
    
    for (int j= i + 1; j < TOTAL_ALPHABET_COUNT; j++) {
      if(rf_config[i] == rf_config[j]) 
        return INVALID_REFLECTOR_MAPPING;
    }
  }
  return NO_ERROR;
}

void Reflector::process_input(int& input) {
  cout << "rf input: " << input << endl;
  // better way of conversion?
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
  // assign values to data members: rot_config[], notch[], num_of_notch

  ifstream in(config_file);
  if (!in) 
    return ERROR_OPENING_CONFIGURATION_FILE;
  int notch_index = 0;
  int count = 0;
  in >> rot_config[count];

  while(!in.eof() && !in.fail()) {
    count++;
    if (count >= TOTAL_ALPHABET_COUNT)
      in >> notch[notch_index++] >> ws;
    else 
      in >> rot_config[count] >> ws;
  }

  num_of_notch = notch_index;

  if(in.fail()) 
    return NON_NUMERIC_CHARACTER;

  // min 26: alphabets 0-25 + 1 notch
  if(count < TOTAL_ALPHABET_COUNT)
    return INVALID_ROTOR_MAPPING;

  for (int i=0; i < TOTAL_ALPHABET_COUNT; i++) {
    if (rot_config[i] < 0 || rot_config[i] > 25)
      return INVALID_INDEX;
    
    for (int j= i + 1; j < TOTAL_ALPHABET_COUNT; j++) {
      if(rot_config[i] == rot_config[j]) 
        return INVALID_ROTOR_MAPPING;
    }
  }

  // check for repeated notch value
  for (int i=0; i < num_of_notch; i++) {
    if (notch[i] < 0 || notch[i] > 25)
      return INVALID_INDEX;
    
    for (int j= i + 1; j < num_of_notch; j++) {
      if(notch[i] == notch[j]) 
        return INVALID_ROTOR_MAPPING;
    }
  }
  return NO_ERROR;
}

void Rotor::set_starting_position(int init) {
  cout << "init: " << init << endl;
  cout << "[0]: " << rot_config[0] << endl;
  cout << "[25]: " << rot_config[25] << endl;  

  if(rot_config[0] == init) 
    return;

  rotate(rot_config);
  return set_starting_position(init);
}

void Rotor::process_input(int& input, bool mapped_backwards) {
  cout << "rot input: " << input << endl;
  if (!mapped_backwards) {
    input = rot_config[input];
  } else 
    input = rot_config[25-input];
  
  rotate(rot_config);
  cout << "rot output: " << input << endl;
}

void Rotor::rotate(int config[]) {
  int last = config[TOTAL_ALPHABET_COUNT - 1];
  int index = TOTAL_ALPHABET_COUNT - 1;
  for (; index >= 0; index--) {
    if (index == 0)
      config[index] = last;
    else config[index] = config[index - 1];
  }
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
        cout << "Error. Res: " << res;
     
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
    if (!in) 
        return ERROR_OPENING_CONFIGURATION_FILE;

    int count;
    for (count=0; count < num_of_rotors && !in.eof() && !in.fail(); count++) {
        in >> starting_pos[count] >> ws;
    }

    if(in.fail()) 
        return NON_NUMERIC_CHARACTER;

    if(count < num_of_rotors)
        return NO_ROTOR_STARTING_POSITION;

    for (int i=0; i < num_of_rotors; i++) {
      if (starting_pos[i] < 0 || starting_pos[i] > 25)
        return INVALID_INDEX;
    }
    return NO_ERROR;
}
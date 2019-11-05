#include <iostream>
#include <fstream>

#include "enigma.h"
#include "errors.h"
using namespace std;

Plugboard::Plugboard (char * config) : config_file(config) {}

int Plugboard::setup() {
  ifstream in(config_file);
  if (!in) 
    return ERROR_OPENING_CONFIGURATION_FILE;
  
  int count, next_ch;
  for (count=0; !in.eof() && !in.fail(); count++) {
    in >> ws;
    next_ch = in.peek();
    in >> pb_config[count] >> ws;
  }

  if (in.fail()) {
    if (next_ch == char_traits<char>::eof())
      count = 0;
    else if (!isdigit(next_ch))
      return NON_NUMERIC_CHARACTER;
  }
  
  num = count;
  if (num > 0) {
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
  if (!in) 
    return ERROR_OPENING_CONFIGURATION_FILE;

  int count, next_ch;
  for (count=0; !in.eof() && !in.fail(); count++) {
    in >> ws;
    next_ch = in.peek();
    in >> rf_config[count] >> ws;
  }

  if (in.fail()) {
    if (!isdigit(next_ch))
      return NON_NUMERIC_CHARACTER;
  }

  int num = count;
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
  if(count < TOTAL_ALPHABET_COUNT) {
    return INVALID_ROTOR_MAPPING;}

  for (int i=0; i < TOTAL_ALPHABET_COUNT; i++) {
    if (rot_config[i] < 0 || rot_config[i] > 25)
      return INVALID_INDEX;
    
    for (int j= i + 1; j < TOTAL_ALPHABET_COUNT; j++) {
      if(rot_config[i] == rot_config[j]) {
        return INVALID_ROTOR_MAPPING;}
    }
  }

  // check for repeated notch value
  for (int i=0; i < num_of_notch; i++) {
    if (notch[i] < 0 || notch[i] > 25)
      return INVALID_INDEX;
    
    for (int j= i + 1; j < num_of_notch; j++) {
      if(notch[i] == notch[j]) {
        return INVALID_ROTOR_MAPPING;}
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
    if (rot_config[0] == notch[i]) {
      return true;
      }
  }
  return false;
}

int prompt_for_input (char input[], int& input_length) {
  char input_arr[MAX_LENGTH];
  cin.getline(input_arr, 80);

  int i, count = 0;
  for (i=0; input_arr[i] != '\0' && i < MAX_LENGTH; i++) {
    if (input_arr[i] == ' ') 
      continue;
    if (input_arr[i] < 'A' || input_arr[i] > 'Z') 
      return INVALID_INPUT_CHARACTER;
    input[count++] = input_arr[i];
  }
  input_length = count;

  return NO_ERROR;

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
        check_error(res, "rotor");
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

void check_error (int res, string source) {
    if (res == 0)
        return;
     
    switch (res) {
        case INSUFFICIENT_NUMBER_OF_PARAMETERS:
            cout << "usage:: enigma plugboard-file reflector-file (<rotor-file>)* rotor-positions\n";
            break;
        case INVALID_INPUT_CHARACTER: 
            cout << "Invalid input character. Input characters must be upper case letters A-Z!\n";
            break;
        case INVALID_INDEX:
            cout << "Invalid index.\n";
            break;
        case NON_NUMERIC_CHARACTER:
            cout << "Non-numeric character in " << source << " file.\n";
            break;
        case IMPOSSIBLE_PLUGBOARD_CONFIGURATION:
            cout << "Impossible plugboard configuration.\n";
            break;
        case INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS:
            cout << "Incorrect number of parameters in plugboard file.\n";
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
            cout << "Incorrect number of parameters in reflector file.\n";
            break;
        case ERROR_OPENING_CONFIGURATION_FILE:
            cout << "Error opening configuration file.\n";
            break;
        default:
            cout << "\n";
    }
    exit(res);
}

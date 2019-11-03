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
  in >> pb_config[count];

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
  in >> rf_config[count];
    
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


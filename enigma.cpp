#include <iostream>
#include <fstream>

#include "enigma.h"
#include "errors.h"
using namespace std;


Plugboard::Plugboard (char * config) : config_file(config) {
}

int Plugboard::setup() {
  // assign values to data members: pb_config[], num

  ifstream in(config_file);
  if (!in) 
    return ERROR_OPENING_CONFIGURATION_FILE;

  int count = 0;
  in >> pb_config[count];

  while(!in.eof()) {
    in >> pb_config[++count] >> ws;
  }
  
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

void Plugboard::process_input(char& input) {
  if (num == 0) 
    return;
  // better way of conversion?
  int letter = input - 'A';

  for (int i=0; i < num; i++) {
    if (pb_config[i] == letter) {
      if (i % 2 == 0)
        input = pb_config[i+1] + 'A';
      else
        input = pb_config[i-1] + 'A';
      return;
    }
  }
}

using namespace std;

int const TOTAL_ALPHABET_COUNT = 26;

class Plugboard {
  char * config_file;
  int pb_config[TOTAL_ALPHABET_COUNT] = {};
  
  public: 
    Plugboard (char * config);
    int num = 0;
    int setup();
    void process_input(char& input);
};

class Reflector {
  char * config_file;
  int rf_config[TOTAL_ALPHABET_COUNT] = {};
  
  public: 
    Reflector (char * config);
    int setup();
    void process_input(char& input);

};

class Rotor {
  char * config_file;
  int rot_config[TOTAL_ALPHABET_COUNT] = {};
  int notch[TOTAL_ALPHABET_COUNT] = {};
  int num_of_notch = 0;
  public: 
    Rotor (char * config);
    int setup();
    void process_input(char& input);

};

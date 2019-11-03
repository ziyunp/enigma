using namespace std;

int const TOTAL_ALPHABET_COUNT = 26;
int const MAX_LENGTH = 100;
int const MIN_PARAMETERS = 4;

class Plugboard {
  char * config_file;
  int pb_config[TOTAL_ALPHABET_COUNT] = {};
  
  public: 
    Plugboard (char * config);
    int num = 0;
    int setup();
    void process_input(int& input);
};

class Reflector {
  char * config_file;
  int rf_config[TOTAL_ALPHABET_COUNT] = {};
  
  public: 
    Reflector (char * config);
    int setup();
    void process_input(int& input);

};

class Rotor {
  // rotor position: 0 = rightmost
  int rotor_position;
  char * config_file;
  int rot_config[TOTAL_ALPHABET_COUNT] = {};
  int notch[TOTAL_ALPHABET_COUNT] = {};
  int num_of_notch = 0;
  void rotate(int config[]);

  public: 
    Rotor (char * config);
    int setup();
    void process_input(int& input, bool mapped_backwards = false);

};

int prompt_for_input (char& input);
void check_error (int res);
Rotor** setup_rotors(int num, char** const argv);

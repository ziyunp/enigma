using namespace std;

int const TOTAL_ALPHABET_COUNT = 26;
int const MAX_LENGTH = 80;
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
  char * config_file;
  int rot_config[TOTAL_ALPHABET_COUNT] = {};
  int notch[TOTAL_ALPHABET_COUNT] = {};
  int num_of_notch = 0;
  void rotate();
  bool rotate_next();
  int notch_position[TOTAL_ALPHABET_COUNT] = {};

  public: 
    Rotor (char * config);
    int setup();
    void set_starting_position(int init);
    bool process_input(int& input, bool rotate_self = false,  bool mapped_backwards = false);

};

void check_error (int res);
Rotor** setup_rotors(int num, char** const argv, int const starting_pos[]);
int open_pos_file(char * pos_file, int num_of_rotors, int starting_pos[]);
void rotors_processing(int& letter, int const num_of_rotors, Rotor** rotors_ptr, bool mapped_backwards);
int process_inputs(char const input[], char output[], int& output_length, int num_of_rotors, Plugboard pb, Rotor** rotors_ptr, Reflector rf, char& error_input);
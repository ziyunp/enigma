using namespace std;

int const TOTAL_ALPHABET_COUNT = 26;
int const MAX_LENGTH = 80;
int const MIN_PARAMETERS = 4;

class Plugboard {
  char * config_file;
  int pb_config[TOTAL_ALPHABET_COUNT] = {};
  int num_of_parameters = 0;
  
  public: 
    Plugboard (char * config);
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
  bool rotate();
  int offset = 0;

  public: 
    Rotor (char * config);
    int setup();
    void set_starting_position(int starting_pos);
    bool process_input(int& input, bool rotate_self = false,  bool mapped_backwards = false);

};

void check_error (int res);

/* 
this function returns ownership of the Rotor pointers to the caller function 
*/
Rotor** setup_rotors(int num_of_rotors, char** const argv, int const starting_pos[]);

int get_starting_pos(char * pos_file, int num_of_rotors, int starting_pos[]);

/* 
this function passes input through all rotors
*/
void rotors_processing(int& letter, int const num_of_rotors, Rotor** rotors_ptr, bool mapped_backwards);

/* 
  this function runs the whole process of encoding / decoding: 
  pb -> rotors -> rf -> rotors(backwards) -> pb 
*/
int process_inputs(char const input[], char output[], int& output_length, int num_of_rotors, Plugboard pb, Rotor** rotors_ptr, Reflector rf, char& error_input);
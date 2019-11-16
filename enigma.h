using namespace std;

int const TOTAL_ALPHABET_COUNT = 26;
int const MAX_LENGTH = 80;
int const MIN_PARAMETERS = 4;

class Plugboard {
  /* A pointer to the plugboard configuration file */
  char * config_file;
  /* An array of integers read out from the plugboard config. file */
  int pb_config[TOTAL_ALPHABET_COUNT] = {};
  /* Number of parameters (integers) contained in the config. file */
  int num_of_parameters = 0;
  
  public: 
    /* 
      Plugboard constructor
      - parameter: pointer to plugboard config. file 
      - initialises the data member char * config_file
    */
    Plugboard (char * config);
    /*  
      This function sets up the configuration of the plugboard
      - assigns value to data members: pb_config, num_of_parameters
      - returns an integer: 0 if NO _ERROR, > 0 otherwise
    */
    int setup();
    /*
      This function processes the input to the plugboard
      - parameter: input
      - input will be modified to the value that it's mapped to
      - input will not be modified if it's not specified in the config
    */
    void process_input(int& input);
};

class Reflector {
  /* A pointer to the reflector configuration file */
  char * config_file;
  /* An array of 26 integers read out from the reflector config. file */
  int rf_config[TOTAL_ALPHABET_COUNT] = {};
  
  public: 
    /* 
      Reflector constructor
      - parameter: pointer to reflector config. file 
      - initialises the data member char * config_file
    */
    Reflector (char * config);
    /*  
      This function sets up the configuration of the reflector
      - assigns value to data members: rf_config
      - returns an integer: 0 if NO _ERROR, > 0 otherwise
    */
    int setup();
    /*
      This function processes the input to the reflector
      - parameter: input
      - every input will be modified to the value that it's mapped to
    */
    void process_input(int& input);
};

class Rotor {
  /* A pointer to the rotor configuration file */
  char * config_file;
  /* An array of 26 integers read out from the rotor config. file */
  int rot_config[TOTAL_ALPHABET_COUNT] = {};
  /* An array of integers representing the positions of notches on the rotor */
  int notch[TOTAL_ALPHABET_COUNT] = {};
  /* Number of notches on the rotor */
  int num_of_notch = 0;
  /* 
    This function rotates the rotor by shifting up 1 position
    - increment by 1 the value of the data member: offset
    - returns true if a notch is triggered i.e. the notch is at the top position
  */
  bool rotate();
  /* 
    offset from the initial index is incremented on each rotation 
    offset is set to 0 when it's rotated 26 times (a full cycle)  
  */
  int offset = 0;

  public: 
    /* 
      Rotor constructor
      - parameter: pointer to rotor config. file 
      - initialises the data member char * config_file
    */
    Rotor (char * config);
    /*  
      This function sets up the configuration of the rotor
      - assigns value to data members: rot_config, notch, num_of_notch
      - returns an integer: 0 if NO _ERROR, > 0 otherwise
    */
    int setup();
    /* 
      This function sets the starting position of the rotor
      - parameter: starting position specified in rotor position file
      - member function rotate() will be called until the specified position is at the top position (index 0)
    */
    void set_starting_position(int starting_pos);
    /* 
      This function processes the input to the rotor
      - parameter: input, rotate_self(if true, call rotate()), mapped_backwards(indicates the direction of input. If true, direction is L-R)
      - every input will be modified to the value that it's mapped to
      - returns true if the notch of this rotor is triggered on rotation
    */
    bool process_input(int& input, bool rotate_self = false,  bool mapped_backwards = false);
};

/**************************** Free Functions ****************************/

/*
  This function gets the starting positions of rotors
  - parameters: rotor position file, num_of_rotors, starting_pos 
  - assigns values to starting_pos, where rotors' starting positions are stored
  - returns an integer: 0 if NO _ERROR, > 0 otherwise
*/
int get_starting_pos(char * pos_file, int num_of_rotors, int starting_pos[]);

/* 
  This function sets up all rotors involved
  - parameters: num_of_rotors, argv (command line arguments), starting_pos
  - constructs instances of Rotor in the heap
  - returns the pointer to an array of rotors (passing ownership to the caller function)
  - returns NULL if num_of_rotors is 0
*/
Rotor** setup_rotors(int num_of_rotors, char** const argv, int const starting_pos[]);

/* 
  This function passes input through all rotors
  - parameters: input, num_of_rotors, pointer to an array of rotors, mapped_backwards (indicates the direction of input. If true, direction is L-R)
  - when mapped_backwards is true, all rotors will not rotate
  - when mapped_backwards is false, the rightmost rotor always rotates and the other rotors may rotate depending on whether the right rotor triggered a notch
*/
void rotors_processing(int& input, int const num_of_rotors, Rotor** rotors_ptr, bool mapped_backwards);

/* 
  This function runs the whole process of encoding / decoding
  - parameters: input array, output array, output_length, num_of_rotors, instance of plugboard, pointer to an array of rotors, instance of reflector, error_input 
  - each input letter is processed in a loop, passing through plugboard -> rotors -> reflector -> rotors(backwards) -> plugboard
  - each modified input is stored in the output array to be printed out
  - output_length allows the assignment of a sentinel character to the end of the output array
  - num_of_rotors could be 0, in which case the input will be passed through plugboard -> reflector -> plugboard 
  - if an invalid input is encountered, it will be stored in error_input to point out to user that it's invalid
  - returns an integer: 0 if NO _ERROR, > 0 otherwise
*/
int process_inputs(char const input[], char output[], int& output_length, int num_of_rotors, Plugboard pb, Rotor** rotors_ptr, Reflector rf, char& error_input);

/* 
  This function checks the resolved value (res) of functions that return an error code
  - parameter: res
  - if a function does not resolve with NO_ERROR, this function will exit the program with the resolved error code 
*/
void check_error (int res);


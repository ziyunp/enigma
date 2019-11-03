using namespace std;

int const TOTAL_ALPHABET_COUNT = 26;

// const int A = 0;
class Plugboard {
  char * config_file;
  int pb_config[TOTAL_ALPHABET_COUNT] = {};
  
  public: 
    Plugboard (char * config);
    int num = 0;
    int setup();
    void process_input(char& input);

};

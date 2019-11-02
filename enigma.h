using namespace std;

enum { A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z };

class Plugboard {
  char * config_file;
  int * config_pairs;
  
  
  public: 
    Plugboard (char * config);
    int num = 0;
    int setup();

};

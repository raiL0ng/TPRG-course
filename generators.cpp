#include<iostream>
#include<vector>
#include<tuple>
#include<cstring>
using namespace std;


struct {
  string filename = "rnd.txt", method_code = "";
  int n = 10000, i = -1;
} Flags_inf;


void linear_congruent_method(vector<int>& seq, int x_0, int a, int c, int m, int n=10000) {
  seq.push_back(x_0);
  for (int i = 1; i < n; i++) {
    seq.push_back((a * seq[i - 1] + c) % m);
  }
}


void additive_method(vector<int>& seq, int n=10000) {

}

void get_key_for_rc4(vector<int>& key, string num) {
  for (int i = 0; i < num.length(); i++) {
    key.push_back((int) num[i] - '0');
  }
}

void rc4(vector<int>& seq, int n=10000) {
  vector<int> k, s(256);
  get_key_for_rc4(k, Flags_inf.i);
  for (int i = 0; i < 256; i++) s[i] = i;
  int i, j = 0, t;
  for (i = 0; i < 256; i++) {
    j = (j + s[i] + k[i]) % 256;
    swap(s[i], s[j]);
  }
  i = 0, j = 0;
  for (int p = 0; p < n; p++) {
    i = (i + 1) % 256;
    j = (j + s[i]) % 256;
    swap(s[i], s[j]);
    t = (s[i] + s[j]) % 256;
    seq.push_back(s[t])
  }
}

vector<int> define_method(string code) {
  vector<int> seq;
  if (code == "lc") {
    linear_congruent_method(seq, 7, 8, 9, 10, Flags_inf.n);
  }
  if (code == "add") {
    
  }
  if (code == "5p") {
    
  }
  if (code == "lfsr") {
    
  }
  if (code == "nfsr") {
    
  }
  if (code == "mt") {
    
  }
  if (code == "rc4") {
    rc4(seq, Flags_inf.n);
  }
  if (code == "rsa") {
    
  }
  if (code == "bbs") {
    
  }
  return seq;
}


bool check_parameters() {
  if (Flags_inf.method_code == "") {
    return false;
  }
  // else if (Flags_inf.i == -1) {
  //   return false;
  // }
  return true;
}
int main(int argc, char* argv[]) {
  string s;

  for (int i = 0; i < argc; i++) {
    // string fl;
    if (argv[i][1] == '/') continue;
    switch (argv[i][1]) {
      case 'g': {
        char * code = (char *) malloc(4);
        int len = strlen(argv[i]) - 3
        strncpy(code, argv[i] + 3, len);
        s = code;
        Flags_inf.method_code = s;
        break;
      }
      case 'i':
        char* num = (char *) malloc(256);
        int len = strlen(argv[i]) - 3
        strncpy(num, argv[i] + 3, len)
        break;
      case 'n': {
        char * n = (char *) malloc(5);
        strncpy(n, argv[i] + 3, strlen(argv[i]) - 3);
        s = n;
        Flags_inf.n = stoi(s);
        if (Flags_inf.n > 10000) {
          cout << "Size of sequence cannot be more 10000!\n";
          return 0;
        }
        break;
      }
      case 'f':
        break;
      case 'h':
        break;
      default:
        cout << "Something wrong... Please enter \'/h\' to see information\n";
        return 0;
    }
  }
  if (!check_parameters()) {
    cout << "Incorrect data! Please enter \'/h\' to see information\n";
    return 0;
  }
  vector<int> seq;
  seq = define_method(Flags_inf.method_code);
  if (seq.size() == 0) {
    cout << "You died!";
  }
  for (int el : seq) {
    cout << el << ' ';
  }
  cout << endl;
  return 0;
}
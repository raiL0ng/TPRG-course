#include<iostream>
#include<vector>
#include<numeric>
#include<cstring>
#include<string>
#include<fstream>
using namespace std;


struct 
{
  string filename = "rnd.txt", method_code = "", i = "";
  int n = 10000;
} Flags_inf;


void linear_congruent_method(vector<int>& seq, vector<int>& p, int n=10000) 
{
  seq.push_back(p[0]);
  for (int i = 1; i < n; i++) 
  {
    seq.push_back((p[1] * seq[i - 1] + p[2]) % p[3]);
  }
}


void additive_method(vector<int>& seq, int n=10000) 
{

}

void get_key_for_rc4(vector<int>& key, string num) {
  for (int i = 0; i < num.length(); i++) {
    if (num[i] != '0' && num[i] != '1') {
      key.clear();
      break;
    } 
    key.push_back((int) num[i] - '0');
  }
}

void rc4(vector<int>& seq, int b_len, int n=10000) 
{
  vector<int> k, s(b_len);
  get_key_for_rc4(k, Flags_inf.i);
  if (k.empty()) {
    return;
  }
  for (int i = 0; i < b_len; i++) s[i] = i;
  int i, j = 0, t;
  for (i = 0; i < b_len; i++) 
  {
    j = (j + s[i] + k[i]) % b_len;
    swap(s[i], s[j]);
  }
  i = 0, j = 0;
  for (int p = 0; p < n; p++) 
  {
    i = (i + 1) % b_len;
    j = (j + s[i]) % b_len;
    swap(s[i], s[j]);
    t = (s[i] + s[j]) % b_len;
    seq.push_back(s[t]);
  }
}


int get_bitrait (int i) 
{
  int n = 1;
  while (i > 1) {
      n *= 10;
      i--;
  }
  return n;
}

bool check_prime(int n) 
{
  if (n == 0 || n == 1) return false;
  for (int i = 2; i < n / 2; i++)
    if (n % i == 0) return false;
  return true;
}

int gen_rand_num(int upp, int low)
{
  srand((unsigned) time(NULL));
  int n = (rand() % (upp - low + 1)) + low;
  while (!check_prime(n))
    n = (rand() % (upp - low + 1)) + low;
  return n;
}

long long power(long long base, long long exp, int mod) 
{
   long long res = 1;
   while (exp > 0) 
   {
      if (exp % 2 == 1)
         res= (res * base) % mod;
      exp = exp >> 1;
      base = (base * base) % mod;
   }
   return res;
}

void rsa(vector<int>& seq, int i, int l=10000)
{
  int low = get_bitrait(i);
  int upp = low * 10 - 1;
  int p = gen_rand_num(upp, low);
  int q = gen_rand_num(upp, low);
  while (p == q)
    q = gen_rand_num(upp, low);
  int n = p * q, f = (p - 1) * (q - 1);
  int e = gen_rand_num(f - 1, 2);
  while (gcd(e, f) != 1)
    e = gen_rand_num(f - 1, 2);
  cout << "p=" << p << " q=" << q << " e=" << e << " n=" << n << endl;
  seq.push_back(gen_rand_num(n - 1, 1));
  int x;
  for (int i = 1; i < l; i++)
  {
    // seq.push_back((power(seq[i - 1], e, n)) % 10);
    seq.push_back(power(seq[i - 1], e, n));

  }
}

void blum_blum_shub_algo(vector<int>& seq, int i, int l=10000)
{
  int low = get_bitrait(i);
  int upp = low * 10 - 1;
  int p = power((rand() % (upp - low + 1)) + low, 3, 4);
  while (!check_prime(p))
    p = power((rand() % (upp - low + 1)) + low, 3, 4);
  int q = gen_rand_num(upp, low);
  while (p == q && (!check_prime(q)))
    q = gen_rand_num(upp, low);
  int n = p * q;
  cout << "p=" << p << " q=" << q << " n=" << n << endl;
  // seq.push_back(gen_rand_num(n - 1, 1));
  int x = (rand() % (upp - low + 1)) + low;
  while (gcd(x, n) != 1)
    x = (rand() % (upp - low + 1)) + low;
  for (int i = 0; i < l; i++)
  {
    // seq.push_back((power(seq[i - 1], e, n)) % 10);

    seq.push_back(power(x, 2, n) & 1);
    x = (rand() % (upp - low + 1)) + low;
  }
}


int convert_parameters(string& s) 
{
  int num = stoi(s.substr(0, s.find(",")));
  s = s.substr(s.find(",") + 1);
  return num;
}


vector<int> define_method(string code) 
{
  vector<int> seq;
  if (code == "lc") {
    vector<int> p(4);
    string params = Flags_inf.i;
    for (int i = 0; i < 4; i++)
      p[i] = convert_parameters(params);
    if ( p[3] <= 0 || p[0] < 0 || p[0] > p[3] || 
         p[1] < 0 || p[1] > p[3] || p[2] < 0 || p[2] > p[3] )
      return seq;
    linear_congruent_method(seq, p, Flags_inf.n);
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
    rc4(seq, Flags_inf.i.length(), Flags_inf.n);
  }
  if (code == "rsa") {
    // TODO доделать параметры i и генерацию чисел
    vector<int> p(2);
    // string params = Flags_inf.i;
    // for (int i = 0; i < 2; i++)
    //   p[i] = convert_parameters(params);
    // if (p[0] >= p[1] || p[0] <= 0 || p[1] <= 0)
    //   return seq;
    int i = stoi(Flags_inf.i);
    if (i <= 0)
      return seq;
    rsa(seq, i, Flags_inf.n);
  }
  if (code == "bbs") {
    // TODO доделать параметры i и генерацию чисел
    blum_blum_shub_algo(seq, stoi(Flags_inf.i), Flags_inf.n);
  }
  return seq;
}


bool check_parameters() 
{
  if (Flags_inf.method_code == "") {
    return false;
  }
  else if (Flags_inf.i == "") {
    return false;
  }
  return true;
}


void advert(string par="") {
  if (par == "") {
    cout << "The program has the following commands:\n";
    cout << "/g:<code_method> --- parameter specifies the"
            " method of IF generations.\ncode_method has the following values:\n";
    cout << "\tlc - linear congruent generator\n\tadd - additive method\n"
            "\t5p - five-parametric method\n\tlfsr - linear-feedback shift register\n"
            "\tnfsr - nonlinear feedback shift register\n\tmt - Mersenne twister\n"
            "\trc4 - RC4\n\trsa - RSA\n\tbbs - Blum-Blum-Shub\'s algorithm\n\n";
    cout << "/i:<parameters> --- generator initialization vector.\n";
    cout << "parameters for methods:\n"
            "\t<X0,a,c,m> - parameters for \"lc\"\n"
            "\t<k,j> - parameters for \"add\"\n\t<---> - parameters for \"5p\"\n"
            "\t<---> - parameters for \"lfsr\"\n\t<---> - parameters for \"nfsr\"\n"
            "\t<---> - parameters for \"mt\"\n"
            "\t<key> - parameter for \"rc4\". key - it's 0 or 1 numbers (1 < their length< 257)\n"
            "\t<---> - parameters for \"rsa\"\n\t<---> - parameters for \"bbs\"\n";
    cout << "/n:<length> --- amount of generated numbers.\n\n";
    cout << "/f:<complete_file_name> --- to output generated numbers to file.\n\n";
    cout << "/h --- complete information about commands.\n\n";
  }
  // TODO
  else if (par == "lc") {
    cout << "The following parameters must be entered for correct operation:\n";
    cout << "/i:<X0,a,c,m> - parameters for \"lc,\"\n where 0 < X0 < n - initial element";
  }
  else if (par == "add") {

  }
  else if (par == "5p") {

  }
  else if (par == "lfsr") {

  }
  else if (par == "nfsr") {

  }
  else if (par == "mt") {

  }
  else if (par == "rc4") {
    cout << "The following parameters must be entered for correct operation:\n\n";
    cout << "/i:<sequence of bits> - sequence of bits have length more then 0 and less then 257\n";
    cout<< "Also you can use /n:<length> to set length of the sequence of numbers 0 < n < 10001\n";
  }
  else if (par == "rsa") {

  }
  else if (par == "bbs") {

  }
}

int main(int argc, char* argv[]) {
  string s;

  for (int i = 0; i < argc; i++) {
    // string fl;
    if (argv[i][1] == '/') continue;
    switch (argv[i][1]) {
      case 'g': {
        char * code = (char *) malloc(2);
        strncpy(code, argv[i] + 3, strlen(argv[i]) - 3);
        s = code;
        Flags_inf.method_code = s;
        break;
      }
      case 'i': {
        char* num = (char *) malloc(256);
        strncpy(num, argv[i] + 3, strlen(argv[i]) - 3);
        Flags_inf.i = num;
        break;
      }
      case 'n': {
        char * n = (char *) malloc(2);
        strncpy(n, argv[i] + 3, strlen(argv[i]) - 3);
        s = n;
        Flags_inf.n = stoi(s);
        if (Flags_inf.n > 10000) {
          cout << "Size of sequence cannot be more 10000!\n";
          return 0;
        }
        break;
      }
      case 'f': {
        char * f = (char *) malloc(5);
        strncpy(f, argv[i] + 3, strlen(argv[i]) - 3);
        Flags_inf.filename = f;
      }
        break;
      case 'h': {
        advert();
        return 0;
      }
        break;
      default: {

        cout << "Something wrong... Please enter \'/h\' to see information\n";
        return 0;
      }
    }
  }
  if (!check_parameters()) {
    cout << "Incorrect data! Please enter \'/h\' to see information\n";
    return 0;
  }
  vector<int> seq;
  seq = define_method(Flags_inf.method_code);
  if (seq.empty()) {
    cout << "Incorrect parameters input!\n";
    advert(Flags_inf.method_code);
    return 0;
  }
  ofstream out;
  out.open(Flags_inf.filename);
  if (out.is_open()) 
  {
    for (int el : seq) 
      out << el << ' ';
  }
  out.close();
  cout << "\nThe sequence of random numbers was generated successfully "
          "and written to the file called rnd.txt\n";
  cout << endl;
  return 0;
}
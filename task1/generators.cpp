#include<iostream>
#include<vector>
#include<numeric>
#include<cstring>
#include<string>
#include<fstream>
#include<cmath>
using namespace std;


struct {
  string filename = "rnd.dat", method_code = "", i = "";
  int n = 10000;
} Flags_inf;


void linear_congruent_method(vector<int>& seq, vector<int>& p, int n=10000) {
  seq.push_back(p[3]);
  for (int i = 1; i < n; i++) {
    seq.push_back((p[1] * seq[i - 1] + p[2]) % p[0]);
  }
}


void additive_method(vector<int>& seq, vector<int>& p, int m, int k, int j, int n=10000) {
  int t = p.size(), num;
  for (int i = 0; i < n; i++) {
    num = (p[t - k] + p[t - j]) % m;
    seq.push_back(num);
    p.push_back(num);
    t++;
  }
    
}

void five_parameter_method( vector<int>&seq, vector<int> xs, int p, int q1
                          , int q2, int q3, int w, int n=10000 ) {
  int num, posl = 0, posr;
  string s;
  vector<int> nums;
  for (int i = 1; i < n; i++) {
    s = "";
    posr = posl + w;
    for (int j = posl; j < posr; j++) {
    // TODO wtf
      num = xs[j + q1] + xs[j + q2] + xs[j + q3] + xs[j];
      s += to_string(num % 2);
      // nums.push_back(num);
      // posl++;
    }
    seq.push_back(stoi(s, nullptr, 2));
  }
  nums.clear();
}
void get_coeffs(vector<int>& key, string num) {
  for (int i = 0; i < num.length(); i++) {
    if (num[i] != '0' && num[i] != '1') {
      key.clear();
      break;
    } 
    key.push_back((int) num[i] - '0');
  }
}


void get_binaryarr(vector<int>& x, int num) {
  int d = 0;
  for (int i = 0; i < 33; i++) {
    if (pow(2, i) > num) {
      d = i;
      break;
    }
  }
  for (int i = d - 1; i >= 0; i--) {
    int k = num >> i;
    if (k & 1)
      x.push_back(1);
    else
      x.push_back(0);
    }
}


void shift_arr(vector<int>& a, int new_el, char fl='l') {
  if (fl == 'l') {
    a.erase(a.begin());
    a.push_back(new_el);
  }
  else {
    vector<int> tmp;
    tmp.push_back(new_el);
    for (int i = 1; i < a.size(); i++)
      tmp.push_back(a[i - 1]);
    a = tmp;
  }
}


void lfsr(vector<int>& seq, vector<int>& coefs, vector<int>& x, int n=10000) {
  int num, len = x.size(), w = 10;
  string s;
  for (int k = 0; k < n; k++) {
    s = "";
    for (int j = 0; j < w; j++) {
      num = coefs[0] * x[0];
      for (int i = 1; i < len; i++)
        num = !num != !(coefs[i] * x[i]);
      s += to_string(num);
      shift_arr(coefs, num, 'r');
    }
    seq.push_back(stoi(s, nullptr, 2));
  }
}

void rc4(vector<int>& seq, vector<int>& k, int b_len, int n=10000) {
  vector<int> s(b_len);
  if (k.empty()) {
    return;
  }
  for (int i = 0; i < b_len; i++) s[i] = i;
  int i, j = 0, t;
  for (i = 0; i < b_len; i++) {
    j = (j + s[i] + k[i]) % b_len;
    swap(s[i], s[j]);
  }
  i = 0, j = 0;
  for (int p = 0; p < n; p++) {
    i = (i + 1) % b_len;
    j = (j + s[i]) % b_len;
    swap(s[i], s[j]);
    t = (s[i] + s[j]) % b_len;
    seq.push_back(s[t]);
  }
}


int get_bitrait (int i) {
  int n = 1;
  while (i > 1) {
      n *= 10;
      i--;
  }
  return n;
}

bool check_prime(int n) {
  if (n == 0 || n == 1) return false;
  for (int i = 2; i < n / 2; i++)
    if (n % i == 0) return false;
  return true;
}

int gen_rand_num(int upp, int low) {
  srand((unsigned) time(NULL));
  int n = (rand() % (upp - low + 1)) + low;
  while (!check_prime(n))
    n = (rand() % (upp - low + 1)) + low;
  return n;
}

long long power(long long base, long long exp, int mod) {
   long long res = 1;
   while (exp > 0) {
      if (exp % 2 == 1)
         res= (res * base) % mod;
      exp = exp >> 1;
      base = (base * base) % mod;
   }
   return res;
}

void rsa(vector<int>& seq, int n, int e, int x0, int l=10000) {
  vector<int> nums;
  int num, bit, posl = 0, posr, b = 10;
  string s;
  cout << "Input parameters:\n";
  cout << "n=" << n << " e=" << e << " x=" << x0 << endl;
  nums.push_back(x0);
  for (int i = 1; i < l; i++) {
    s = "";
    posr = posl + b;
    for (int j = posl; j < posr; j++) {
      num = power(nums[j - 1], e, n);
      s += to_string(num & (1 << 0));
      nums.push_back(num);
      posl++;
    }
    seq.push_back(stoi(s, nullptr, 2));
  }
  nums.clear();
}

void blum_blum_shub_algo(vector<int>& seq, int x0, int l=10000) {
  int num, n = 16637, posl = 0, posr, b = 10;
  string s;
  vector<int> nums;
  nums.push_back(x0);
  for (int i = 1; i < l; i++) {
    s = "";
    posr = posl + b;
    for (int j = posl; j < posr; j++) {
      num = power(nums[j - 1], 2, n);
      s += to_string(num & (1 << 0));
      nums.push_back(num);
      posl++;
    }
    seq.push_back(stoi(s, nullptr, 2));
  }
  nums.clear();
}


int convert_parameters(string& s) {
  int num = stoi(s.substr(0, s.find(",")));
  string tmp = s.substr(s.find(",") + 1);
  s = tmp;
  return num;
}

void find_multiply_of_number(int aM1, int m) {
  int k = 2;
  while (k <= m / 2) {
    if (m % k == 0 && check_prime(k) && aM1 % k == 0)
      break;
    k++;
  }
  if (k <= m / 2)
    cout << "a - 1 is a multiple of p = " << k << " that "
            "is a divisor of m\n";
  else
    cout << "There is no prime number p such that a is a multiple of p\n";
}


vector<int> define_method(string code) {
  vector<int> seq;
  if (code == "lc") {
    vector<int> p(4);
    string params = Flags_inf.i;
    for (int i = 0; i < 4; i++)
      p[i] = convert_parameters(params);
    if ( p[0] <= 0 || p[3] < 0 || p[3] > p[0] || 
         p[1] < 0 || p[1] > p[0] || p[2] < 0 || p[2] > p[0] )
      return seq;
    cout << "m = " << p[0] << " a = " << p[1] << " c ="
            " " << p[2] << " x0 = " << p[3] << endl;
    if (gcd(p[2], p[0]) != 1)
      cout << "m and c are not mutually prime\n";
    else
      cout << "m and c are mutually prime\n";
    find_multiply_of_number(p[1] - 1, p[0]);
    if ((p[1] - 1) % 4 == 0 && p[0] % 4 == 0)
      cout << "a - 1 is a multiply of 4 and m is a multiply of 4\n";
    else {
      if (p[0] % 4 == 0)
        cout << "a - 1 is not a multiply of 4\n";
      else if ((p[1] - 1) % 4 == 0)
        cout << "m is not a multiply of 4\n";
      else
        cout << "a - 1 is not a multiply of 4 and m is not a multiply of 4\n";
    }
    linear_congruent_method(seq, p, Flags_inf.n);
  }
  if (code == "add") {
    vector<int> p;
    string params = Flags_inf.i;
    int tmp, m, k, j;
    m = convert_parameters(params);
    k = convert_parameters(params);
    j = convert_parameters(params);
    p.push_back(convert_parameters(params));
    p.push_back(convert_parameters(params));
    for (int i = 2; i < 56; i++) {
      int tmp = convert_parameters(params);
      if ((tmp == p[i - 1] && tmp == p[i - 2])) {
        p.pop_back();
        break;
      }
      p.push_back(tmp);
    }
    if (p.size() > 55)
      p.pop_back();
    if (k >= j || k < 1 || j < 1 || k > p.size() - 3 || j > p.size() - 3)
      return seq;
    additive_method(seq, p, m, k, j, Flags_inf.n);
  }
  if (code == "5p") {
    
  }
  if (code == "lfsr") {
    int x;
    vector<int> c, xs;
    string params = Flags_inf.i;
    get_coeffs(c, params.substr(0, params.find(",")));
    x = convert_parameters(params);
    x = convert_parameters(params);
    get_binaryarr(xs, x);
    // TODO подумать над ограничениями
    if (xs.size() > c.size())
      return seq;
    for (int el : c) cout << el; cout<<endl;
    for (int el : xs) cout << el; cout<<endl;
    
    lfsr(seq, c, xs, Flags_inf.n);
  }
  if (code == "nfsr") {
    
  }
  if (code == "mt") {
    
  }
  if (code == "rc4") {
    vector<int> p;
    int num;
    string params = Flags_inf.i;
    p.push_back(convert_parameters(params));
    p.push_back(convert_parameters(params));
    for (int i = 2; i < 257; i++) {
      int tmp = convert_parameters(params);
      if ((tmp == p[i - 1] && tmp == p[i - 2])) {
        p.pop_back();
        break;
      }
      p.push_back(tmp);
    }
    if (p.size() > 256) p.pop_back();
    rc4(seq, p, p.size(), Flags_inf.n);
  }
  if (code == "rsa") {
    int n, e, x;
    string params = Flags_inf.i;
    n = convert_parameters(params);
    e = convert_parameters(params);
    x = convert_parameters(params);
    if (1 > x || x > n)
      return seq;
    rsa(seq, n, e, x, Flags_inf.n);
  }
  if (code == "bbs") {
    // TODO доделать параметры i и генерацию чисел
    int x0 = stoi(Flags_inf.i);
    if (gcd(x0, 16637) != 1) {
      cout << "x0=" << x0 << " and n=16637 must be reciprocal!\n";
      return seq;
    }
    blum_blum_shub_algo(seq, x0, Flags_inf.n);
  }
  return seq;
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
            "\t<m,k,j,x_1,...x_55> - parameters for \"add\"\n"
            "\t<p,q1,q2,q3,w,x_1...x_p> - parameters for \"5p\"\n"
            "\t<coeffs,num> - parameters for \"lfsr\"\n\t<---> - parameters for \"nfsr\"\n"
            "\t<---> - parameters for \"mt\"\n"
            "\t<seq> - parameter for \"rc4\". seq - sequence of numbers (1 < sequence length< 257)\n"
            "\t<n,e,x> - parameters for \"rsa\"\n\t<x> - parameter for \"bbs\"\n";
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
    //TODO
    cout << "The following parameters must be entered for correct operation:\n\n";
    cout << "/i:<coeffs,num> - coeffs - sequens of 0 and 1, num - decimal number\n";
    cout<< "Also you can use /n:<length> to set length of the sequence of numbers 0 < n < 10001\n";
  }
  else if (par == "nfsr") {

  }
  else if (par == "mt") {

  }
  else if (par == "rc4") {
    cout << "The following parameters must be entered for correct operation:\n\n";
    cout << "/i:<sequence of numbers> - sequence of numbers have length more then 0 and less then 257\n";
    cout<< "Also you can use /n:<length> to set length of the sequence of numbers 0 < n < 10001\n";
  }
  else if (par == "rsa") {
    cout << "The following parameters must be entered for correct operation:\n\n";
    cout << "/i:<n,e,x> - where n - module, e - number for to exponentiate," 
            " x - initial number (0 < x < n + 1).\n";
    cout<< "Also you can use /n:<length> to set length of the sequence of numbers 0 < n < 10001\n";
  }
  else if (par == "bbs") {
    cout << "The following parameters must be entered for correct operation:\n\n";
    cout << "/i:<x> - where x - initial number (0 < x < n + 1) and x must be GCD(x, 16637) = 1.\n";
    cout<< "Also you can use /n:<length> to set length of the sequence of numbers 0 < n < 10001\n";
  }
}


bool check_parameters() 
{
  if (Flags_inf.method_code == "") {
    cout << "Incorrect data! Please enter \'/h\' to see information\n";
    return false;
  }
  else if (Flags_inf.method_code != "" && Flags_inf.i == "") {
    cout << "Incorrect data! Please enter \'/h\' to see information\n";
    advert(Flags_inf.method_code);
    return false;
  }
  return true;
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
  if (out.is_open()) {
    for (int i = 0; i < Flags_inf.n - 1; i++)
      out << seq[i] << ',';
    out << seq[Flags_inf.n - 1];
  }
  out.close();
  cout << "\nThe sequence of random numbers was generated successfully "
          "and written to the file called rnd.dat\n";
  cout << endl;
  return 0;
}
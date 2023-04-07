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

// For mersenne twister method
const int mt_p = 624;
const int mt_u = 11;
const int mt_s = 7;
const int mt_t = 15;
const int mt_l = 18;
const int mt_q = 397;
const long mt_a = 2567483615;
const long mt_b = 2636928640;
const long mt_c = 4022730752;
const long mt_d = 4294967295;
const long upp_mask = 0x80000000;
const long low_mask = 0x7fffffff;


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


void five_parameter_method( vector<int>&seq, vector<int>& xs, int p, int q1
                          , int q2, int q3, int w, int n=10000 ) {
  int num, k = 0;
  string s;
  vector<int> nums;
  for (int i = 1; i < n; i++) {
    s = "";
    for (int j = 0; j < w; j++) {
      num = (xs[k + q1] ^ xs[k + q2] ^ xs[k + q3] ^ xs[k]) & 1;
      k++;
      s += to_string(num);
      xs.push_back(num);
    }
    seq.push_back(stoi(s, nullptr, 2));
  }
  nums.clear();
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
      for (int i = 1; i < len; i++) {
        if (x[i])
          num ^= coefs[i];
      }
      s += to_string(num);
      shift_arr(coefs, num, 'r');
    }
    seq.push_back(stoi(s, nullptr, 2));
  }
}


int get_bit_nfsr(vector<int>& r, int n) {
  int b = r[0];
  for (int i = 1; i < n; i++) 
    b ^= r[i];
  shift_arr(r, b, 'l');
  return b;
}


void nfsr(vector<int>& seq, vector<int>& r1, vector<int>& r2, vector<int>& r3, int n=10000) {
  int b1, b2, b3, w = 10;
  int n1 = r1.size(), n2 = r2.size(), n3 = r3.size();
  string s;
  for (int k = 0; k < n; k++) {
    s = "";
    for (int j = 0; j < w; j++) {
      b1 = get_bit_nfsr(r1, n1);
      b2 = get_bit_nfsr(r2, n2);
      b3 = get_bit_nfsr(r3, n3);
      s += to_string((b1 & b2) ^ (b2 & b3) ^ b3);
    }
    seq.push_back(stoi(s, nullptr, 2));
  }
}


long get_kth_lowest_bits(long long num, int k) {
  return (((1L << k) - 1) & num);
}

template<typename T>
int count_bits(T num) {
  int c = 0;
  for (num; num >>= 1;)
    c += 1;
  return c;
}


void initialize_mt(vector<long>& x, int p, int x0, int w) {
  long long f = 1812433253, num;
  x.push_back(x0);
  for (int i = 1; i < p; i++) {
    num = (f * (x[i - 1] ^ (x[i - 1] >> (w - 2)))) + i;
    x.push_back(get_kth_lowest_bits(num, w));
  }
}


void mt(vector<int>& seq, vector<long> xs, int m, int n=10000) {
  int ind = mt_p;
  for (int cnt = 0; cnt < n; cnt++) {
    if (ind >= mt_p) {
      for (int i = 0; i < mt_p; i++) {
        long num = (xs[i] & upp_mask) + (xs[(i + 1) % mt_p] & low_mask);
        long xA = num >> 1;
        if (num & 1) xA ^= mt_a;
        xs[i] =xs[(i + mt_q) % mt_p] ^ xA;
      }
      ind = 0;
    }
    long y = xs[ind];
    y ^= ((y >> mt_u) & mt_d);
    y ^= ((y << mt_s) & mt_b);
    y ^= ((y << mt_t) & mt_c);
    y ^= (y >> 1);
    seq.push_back(y % m);
    ind++;
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


// is not used
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
  int num, w = 10;
  string s;
  cout << "Input parameters:\n";
  cout << "n=" << n << " e=" << e << " x=" << x0 << endl;
  num = x0;
  for (int i = 1; i < l; i++) {
    s = "";
    for (int j = 0; j < w; j++) {
      num = power(num, e, n);
      s += to_string(num & 1);
    }
    seq.push_back(stoi(s, nullptr, 2));
  }
}


void blum_blum_shub_algo(vector<int>& seq, int x0, int l=10000) {
  int num, n = 16637, w = 10;
  string s;
  num = x0;
  for (int i = 1; i < l; i++) {
    s = "";
    for (int j = 0; j < w; j++) {
      num = power(num, 2, n);
      s += to_string(num & (1 << 0));
    }
    seq.push_back(stoi(s, nullptr, 2));
  }
}


void get_coeffs_from_string(vector<int>& key, string& s) {
  string num = s.substr(0, s.find(","));
  for (int i = 0; i < num.length(); i++) {
    if (num[i] != '0' && num[i] != '1') {
      key.clear();
      break;
    } 
    key.push_back((int) num[i] - '0');
  }
  s = s.substr(s.find(",") + 1);
}


int convert_parameters(string& s) {
    int num = stoi(s.substr(0, s.find(",")));
    s = s.substr(s.find(",") + 1);
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
    int m, k, j;
    m = convert_parameters(params);
    k = convert_parameters(params);
    j = convert_parameters(params);
    for (int i = 0; i < 56; i++) {
      if (params.find(",") == -1) {
        p.push_back(convert_parameters(params));
        break;
      }
      p.push_back(convert_parameters(params));
    }
    if (k >= j || k < 1 || j < 1 || k > p.size() - 3 || j > p.size() - 3)
      return seq;
    additive_method(seq, p, m, k, j, Flags_inf.n);
  }
  if (code == "5p") {
    vector<int> xs;
    string params = Flags_inf.i;
    int p, q1, q2, q3, w;
    p = convert_parameters(params);
    q1 = convert_parameters(params);
    q2 = convert_parameters(params);
    q3 = convert_parameters(params);
    w = convert_parameters(params);
    for (int i = 0; i < p; i++) {
      if (params.find(",") == -1) {
        xs.push_back(convert_parameters(params));
        break;
      }
      xs.push_back(convert_parameters(params));
    }
    five_parameter_method(seq, xs, p, q1, q2, q3, w, Flags_inf.n);
  }
  if (code == "lfsr") {
    int x;
    vector<int> c, xs;
    string params = Flags_inf.i;
    get_coeffs_from_string(c, params);
    x = convert_parameters(params);
    get_binaryarr(xs, x);
    if (xs.size() > c.size())
      return seq;
    lfsr(seq, c, xs, Flags_inf.n);
  }
  if (code == "nfsr") {
    vector<int> r1, r2, r3;
    string params = Flags_inf.i;
    get_coeffs_from_string(r1, params);
    get_coeffs_from_string(r2, params);
    get_coeffs_from_string(r3, params);
    nfsr(seq, r1, r2, r3, Flags_inf.n);

  }
  if (code == "mt") {
    string params = Flags_inf.i;
    int m = convert_parameters(params);
    int x = convert_parameters(params);
    vector<long> xs;
    initialize_mt(xs, mt_p, x, count_bits(x));
    mt(seq, xs, m, Flags_inf.n);
    // for (long el : xs) cout << el << ' '; cout << endl;
  }
  if (code == "rc4") {
    vector<int> p;
    string params = Flags_inf.i;
    for (int i = 0; i < 257; i++) {
      if (params.find(",") == -1) {
        p.push_back(convert_parameters(params) % 256);
        break;
      }
      p.push_back(convert_parameters(params) % 256);
    }
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
            "\t<coeffs,num> - parameters for \"lfsr\"\n"
            "\t<r1,r2,r3> - parameters for \"nfsr\"\n"
            "\t<m, x> - parameters for \"mt\"\n"
            "\t<seq> - parameter for \"rc4\". seq - sequence of numbers (1 < sequence length< 257)\n"
            "\t<n,e,x> - parameters for \"rsa\"\n\t<x> - parameter for \"bbs\"\n";
    cout << "/n:<length> --- amount of generated numbers.\n\n";
    cout << "/f:<complete_file_name> --- to output generated numbers to file.\n\n";
    cout << "/h --- complete information about commands.\n\n";
  }
  else if (par == "lc") {
    cout << "The following parameters must be entered for correct operation:\n";
    cout << "/i:<X0,a,c,m> - parameters for \"lc\",\n where 0 < X0 < n - initial element";
    cout<< "Also you can use /n:<length> to set length of the sequence of numbers 0 < n < 10001\n";
  }
  else if (par == "add") {
    cout << "The following parameters must be entered for correct operation:\n";
    cout << "/i:<m,k,j,x_1,x_2,...,x_55> - parameters for \"add\",\n "
            "where m - the module, k and j (0 < k < j) - indicies of numbers x_1,x_2,...,x_55";
    cout<< "Also you can use /n:<length> to set length of the sequence of numbers 0 < n < 10001\n";
  }
  else if (par == "5p") {
    cout << "The following parameters must be entered for correct operation:\n";
    cout << "/i:<p,q_1,q_2,q_3,w,x_1,x_2,...,x_p> - parameters for \"5p\",\n "
            "where p - length of numbers x_1,x_2,...,x_p, q_1, q_2 and q_3"
            " - indicies of those numbers x_1,x_2,...,x_55, w - bits number";
    cout<< "Also you can use /n:<length> to set length of the sequence of numbers 0 < n < 10001\n";
  }
  else if (par == "lfsr") {
    cout << "The following parameters must be entered for correct operation:\n\n";
    cout << "/i:<coeffs,num> - coeffs - sequence of 0 and 1, num - decimal number\n";
    cout<< "Also you can use /n:<length> to set length of the sequence of numbers 0 < n < 10001\n";
  }
  else if (par == "nfsr") {
    cout << "The following parameters must be entered for correct operation:\n\n";
    cout << "/i:<r1,r2,r3> - r1,r2 and r3 - sequences of 0 and 1 numbers\n";
    cout<< "Also you can use /n:<length> to set length of the sequence of numbers 0 < n < 10001\n";
  }
  else if (par == "mt") {
    cout << "The following parameters must be entered for correct operation:\n\n";
    cout << "/i:<m, x> - m - module, x - initial number\n";
    cout<< "Also you can use /n:<length> to set length of the sequence of numbers 0 < n < 10001\n";
  }
  else if (par == "rc4") {
    cout << "The following parameters must be entered for correct operation:\n\n";
    cout << "/i:<sequence of numbers> - sequence of numbers have length more then 0 and less then 257\n";
    cout<< "Also you can use /n:<length> to set length of the sequence of numbers 0 < n < 10001\n";
  }
  else if (par == "rsa") {
    cout << "The following parameters must be entered for correct operation:\n\n";
    cout << "/i:<m,e,x> - where m - module, e - number for to exponentiate," 
            " x - initial number (0 < x < m + 1).\n";
    cout<< "Also you can use /n:<length> to set length of the sequence of numbers 0 < n < 10001\n";
  }
  else if (par == "bbs") {
    cout << "The following parameters must be entered for correct operation:\n\n";
    cout << "/i:<x> - where x - initial number (0 < x < 16638) and x must be GCD(x, 16637) = 1.\n";
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
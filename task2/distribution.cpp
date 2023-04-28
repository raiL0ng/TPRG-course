#include<iostream>
#include<vector>
#include<numeric>
#include<cstring>
#include<string>
#include<fstream>
#include<cmath>
using namespace std;

struct {
  string fileNameRead = "", method_code = "";
  double p1 = -100000, p2 = -100000, p3 = -100000;
  int n;
  double nmax = -1.0;
} Flags_inf;

void get_U(vector<double>& u, int n, double mx) {
  for (int i = 0; i < n; i++) {
    u[i] /= mx;
  }
}

void standard_dist(vector<double>& u, int n, double a, double b) {
  for (int i = 0; i < n; i++) {
    u[i] = b * u[i] + a;
  }
}

void triangle_dist(vector<double>& u, int n, double a, double b) {
  double u0 = u[0];
  for (int i = 0; i < n; i++) {
    u[i] = a + b * (u[i] + u[i + 1] - 1);
  }
  u[n - 1] = a + b * (u[n - 1] + u0 - 1);
}

void exponential_dist(vector<double>& u, int n, double a, double b) {
  for (int i = 0; i < n; i++) {
    u[i] = -b * log(u[i]) + a;
  }
}


void normal_dist(vector<double>& u, int n, double mu, double sigma) {
  for (int i = 0; i < n - 1; i += 2) {
        double u1 = u[i];
        u[i] = mu + sigma * sqrt(-2.0 * log(1 - u[i])) * cos(2.0 * M_PI * u[i + 1]);
        u[i + 1] = mu + sigma * sqrt(-2.0 * log(1 - u1)) * sin(2.0 * M_PI * u[i + 1]);
    }
}


void gamma_dist(vector<double>& u, int n, double a, double b, double c) {
  vector<double> t(u);
  if (floor(c) != c) {
    c = floor(c);
    cout << "The third parameter (p3) must be an integer! Therefore, the number c = " << c << endl;
  }
  else {
    double tmp_mult;
    for (int i = 0; i < n; i++) {
      tmp_mult = 1;
      for (int j = 0; j < c; j++) tmp_mult *= 1 - t[(i + j) % n];
      u[i] = a - b * log(tmp_mult);
    }
  }
}


void lognormal_dist(vector<double>& u, int n, double a, double b) {
  normal_dist(u, n, a, b);
  for (int i = 0; i < n; i++)
    u[i] = a + exp(b - u[i]);
}


void logistic_dist(vector<double>& u, int n, double a, double b) {
  for (int i = 0; i < n; i++)
    u[i] = a + b * log(u[i] / (1 - u[i]));
}


int binomialCoeff(int n, int k) {
  if (k > n)
    return 0;
  if (k == 0 || k == n)
    return 1;
  return binomialCoeff(n - 1, k - 1) + binomialCoeff(n - 1, k);
}


void binomial_dist(vector<double>& u, int n, double a, double b) {
  double s;
  int k;
  for (int i = 0; i < n; i++) {
    s = 0.0;
    k = 0;
    while (true) {
      s += binomialCoeff(b, k) * pow(a, k) * pow((1 - a), b - k);
      if (s > u[i]) {
        u[i] = k;
        break;
      }
      if (k < b - 1) {
        k++;
        continue;
      }
      u[i] = b;
    }
  }
}


bool define_method(vector<double>& dist, string code) {
  double eps = 0.00000001;
  if (code == "st") {
    standard_dist(dist, Flags_inf.n, Flags_inf.p1, Flags_inf.p2);
  }
  else if (code == "tr") {
    triangle_dist(dist, Flags_inf.n, Flags_inf.p1, Flags_inf.p2);
  }
  else if (code == "ex") {
    exponential_dist(dist, Flags_inf.n, Flags_inf.p1, Flags_inf.p2);
  }
  else if (code == "nr") {
    normal_dist(dist, Flags_inf.n, Flags_inf.p1, Flags_inf.p2);
  }
  else if (code == "gm") {
    if (fabs(Flags_inf.p3 + 100000) < eps)
      return false;
    gamma_dist(dist, Flags_inf.n, Flags_inf.p1, Flags_inf.p2, Flags_inf.p3);
  }
  else if (code == "ln") {
    lognormal_dist(dist, Flags_inf.n, Flags_inf.p1, Flags_inf.p2);
  }
  else if (code == "ls") {
    logistic_dist(dist, Flags_inf.n, Flags_inf.p1, Flags_inf.p2);
  }
  else if (code == "bi") {
    binomial_dist(dist, Flags_inf.n, Flags_inf.p1, Flags_inf.p2);
  }
  if (dist.empty())
    return false;
  return true;
}


void advert(string par="") {
  if (par == "") {
    cout << "The program has the following commands:\n";
    cout << "/d:<code_distribution> --- allocation code for sequence conversion.\n"
            "code_method has the following values:\n";
    cout << "\tst - standard even distribution\n"
            "\ttr - triangle distribution\n\tex - exponential distribution\n"
            "\tnr - normal distribution\n\tgm - gamma distribution\n"
            "\tln - lognormal distribution\n"
            "\tls - logistic distribution\n"
            "\tbi - binomial distribution\n";
    cout << "\nParameters for methods:\n";
    cout << "/p1:<a> --- first parameter\n/p2:<b> --- second parameter\n"
            "/p3:<c> --- third parameter (needs only for gamma distribution)\n\n";
    cout << "/f:<complete_file_name> --- to get generated numbers from file.\n\n";
    cout << "/h --- complete information about commands.\n\n";
  }
  else if ( par == "st" || par == "tr" || par == "nr" || 
            par == "ln" || par == "ls" || par == "bi") {
    cout << "The following parameters must be entered for correct operation:\n";
    cout << "/p1:<a> --- first parameter\n/p2:<b> --- second parameter\n";
    cout << "/f:<complete_file_name> --- to get generated numbers from file.\n\n";
  }
  else if (par == "gm") {
    cout << "/p1:<a> --- first parameter\n/p2:<b> --- second parameter\n"
            "/p3<c> --- third parameter";
    cout << "/f:<complete_file_name> --- to get generated numbers from file.\n\n";
    cout << "/h --- complete information about commands.\n\n";
  }
}

double convert_parameters(string& s) {
    double num = stod(s.substr(0, s.find(",")));
    s = s.substr(s.find(",") + 1);
    return num;
}


bool check_parameters() 
{
  if (Flags_inf.method_code == "") {
    cout << "Incorrect data! Please enter \'/h\' to see information\n";
    return false;
  }
  if (Flags_inf.method_code != "" && (Flags_inf.p1 == -100000 || Flags_inf.p2 == -100000)) {
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
      case 'd': {
        s = argv[i];
        Flags_inf.method_code = s.erase(0, 3);
        break;
      }
      case 'p': {
        switch (argv[i][2]){
        case '1':
          s = argv[i];
          Flags_inf.p1 = stod(s.erase(0, 4));
          break;
        case '2':
          s = argv[i];
          Flags_inf.p2 = stod(s.erase(0, 4));
          break;
        case '3':
          s = argv[i];
          Flags_inf.p3 = stod(s.erase(0, 4));
          break;        
        }
        break;
      }
      case 'f': {
        s = argv[i];

        Flags_inf.fileNameRead = s.erase(0, 3);
      }
        break;
      case 'h': {
        advert();
        return 0;
      }
        break;
    }
  }
  if (!check_parameters()) {
    return 0;
  }
  ifstream fin;
  string seq;
  try {
    fin.open(Flags_inf.fileNameRead);
    while (!fin.eof()) {
      fin >> seq;
    }
  }
  catch (const ifstream::failure& e) {
    cout << "Some problem with file..." << Flags_inf.fileNameRead << "!\n";
  }
  fin.close();
  vector<double> dist;
  double tmp, nmax;
  while (true) {
    if (seq.find(",") == -1) {
      tmp = convert_parameters(seq);
      if (nmax < tmp) nmax = tmp;
      dist.push_back(tmp);
      break;
    }
    tmp = convert_parameters(seq);
    if (nmax < tmp) nmax = tmp;
    dist.push_back(tmp);
  }
  Flags_inf.nmax = nmax + 1;
  Flags_inf.n = dist.size();
  cout << Flags_inf.n << " numbers was read.\n";
  cout << "Maximal number of secuence: " << Flags_inf.nmax << endl;
  get_U(dist, Flags_inf.n, Flags_inf.nmax);
  bool fl = define_method(dist, Flags_inf.method_code);
  if (dist.empty()) {
    cout << "Incorrect parameters input!\n";
    advert(Flags_inf.method_code);
    return 0;
  }
  if (!fl) {
    advert(Flags_inf.method_code);
  }
  else {
    string fout = "distr-" + Flags_inf.method_code + ".dat";
    ofstream out;
    out.open(fout);
    if (out.is_open()) {
      for (int i = 0; i < Flags_inf.n - 1; i++)
        out << dist[i] << ',';
      out << dist[Flags_inf.n - 1];
    }
    out.close();
    cout << "\nThe sequence of random numbers was generated successfully "
            "and written to the file called " << fout << "\n";
    cout << endl;
  }
  return 0;
}
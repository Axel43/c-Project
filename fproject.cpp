#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;
#define MIN_suma 500
class FonduriInsuficiente
{
};
class Account
{
private:
  long nrCont;
  string nume;
  string prenume;
  float suma;
  static long nrContUrm;

public:
  Account() {}
  Account(string nume, string prenume, float suma)
  {
    nrContUrm++;
    nrCont = nrContUrm;
    this->nume = nume;
    this->prenume = prenume;
    this->suma = suma;
  }
  long getNrCont() { return nrCont; }
  string getNume() { return nume; }
  string getPrenume() { return prenume; }
  float getSuma() { return suma; }
  void Depune(float cant)
  {
    suma += cant;
  }
  void Retragere(float cant)
  {
    if (suma - cant < MIN_suma)
      throw FonduriInsuficiente();
    suma -= cant;
  }
  static void setLastnrCont(long nrCont)
  {
    nrContUrm = nrCont;
  }
  static long getLastnrCont()
  {
    return nrContUrm;
  }
  friend ofstream &operator<<(ofstream &ofs, Account &acc);
  friend ifstream &operator>>(ifstream &ifs, Account &acc);
  friend ostream &operator<<(ostream &os, Account &acc);
};
long Account::nrContUrm = 0;
class Bank : public Account
{
private:
  map<long, Account> accounts;

public:
  Bank()
  {
    Account account;
    ifstream infile;
    infile.open("Banca.data");
    if (!infile)
    {
      cout << "Eroare in deschiderea contului" << endl;
      return;
    }
    while (!infile.eof())
    {
      infile >> account;
      accounts.insert(pair<long, Account>(account.getNrCont(), account));
    }
    Account::setLastnrCont(account.getNrCont());
    infile.close();
  }
  Account OpenAccount(string nume, string prenume, float suma)
  {
    ofstream outfile;
    Account account(nume, prenume, suma);
    accounts.insert(pair<long, Account>(account.getNrCont(), account));
    outfile.open("Banca.data", ios::trunc);
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
      outfile << itr->second;
    }
    outfile.close();
    return account;
  }
  Account sumaEnquiry(long nrCont)
  {
    map<long, Account>::iterator itr = accounts.find(nrCont);
    return itr->second;
  }
  Account Depune(long nrCont, float cant)
  {
    map<long, Account>::iterator itr = accounts.find(nrCont);
    itr->second.Depune(cant);
    return itr->second;
  }
  Account Retragere(long nrCont, float cant)
  {
    map<long, Account>::iterator itr = accounts.find(nrCont);
    itr->second.Retragere(cant);
    return itr->second;
  }
  void CloseAccount(long nrCont)
  {
    map<long, Account>::iterator itr = accounts.find(nrCont);
    cout << "Cont Sters" << itr->second;
    accounts.erase(nrCont);
  }
  void ShowAllAccounts()
  {
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
      cout << "Cont " << itr->first << endl
           << itr->second << endl;
    }
  }
  ~Bank()
  {
    ofstream outfile;
    outfile.open("Banca.data", ios::trunc);
    map<long, Account>::iterator itr;
    for (itr = accounts.begin(); itr != accounts.end(); itr++)
    {
      outfile << itr->second;
    }
    outfile.close();
  }
};
int main()
{
  Bank b;
  Account acc;
  int optiune;
  string nume, prenume;
  long nrCont;
  float suma;
  float cant;
  cout << "***********************************" << endl;
  do
  {
    cout << "\n\tAlege una dintre optiuni ";
    cout << "\n\t1 Deschide cont ";
    cout << "\n\t2 Afla soldul din cont ";
    cout << "\n\t3 Depune o suma  ";
    cout << "\n\t4 Retrage o suma";
    cout << "\n\t5 Inchide un cont ";
    cout << "\n\t6 Arata toate conturile existente ";
    cout << "\n\t7 Inchide BANCA";
    cout << "\nIntrudu optiunea ta: ";
    cin >> optiune;
    switch (optiune)
    {
    case 1:
      cout << "Introdu Nume: ";
      cin >> nume;
      cout << "Introdu prenume: ";
      cin >> prenume;
      cout << "Introdu o suma intiala: ";
      cin >> suma;
      acc = b.OpenAccount(nume, prenume, suma);
      cout << endl
           << "Felicitari ai creat un cont" << endl;
      cout << acc;
      break;
    case 2:
      cout << "Introdu numarul contului: ";
      cin >> nrCont;
      acc = b.sumaEnquiry(nrCont);
      cout << endl
           << "Detaliile contului tau" << endl;
      cout << acc;
      break;
    case 3:
      cout << "Introdu numarul contului: ";
      cin >> nrCont;
      cout << "Introdu suma:";
      cin >> cant;
      acc = b.Depune(nrCont, cant);
      cout << endl
           << "Suma a fost depusa" << endl;
      cout << acc;
      break;
    case 4:
      cout << "Introdu numar cont:";
      cin >> nrCont;
      cout << "Introdu suma:";
      cin >> cant;
      acc = b.Retragere(nrCont, cant);
      cout << endl
           << "Suma a fost retrasa" << endl;
      cout << acc;
      break;
    case 5:
      cout << "Introdu numar cont:";
      cin >> nrCont;
      b.CloseAccount(nrCont);
      cout << endl
           << "Contul a fost inchis" << endl;
      cout << acc;
    case 6:
      b.ShowAllAccounts();
      break;
    case 7:
      break;
    default:
      cout << "\nIntrodu o optiune corecta ";
      exit(0);
    }
  } while (optiune != 7);
  return 0;
}
ofstream &operator<<(ofstream &ofs, Account &acc)
{
  ofs << acc.nrCont << endl;
  ofs << acc.nume << endl;
  ofs << acc.prenume << endl;
  ofs << acc.suma << endl;
  return ofs;
}
ifstream &operator>>(ifstream &ifs, Account &acc)
{
  ifs >> acc.nrCont;
  ifs >> acc.nume;
  ifs >> acc.prenume;
  ifs >> acc.suma;
  return ifs;
}
ostream &operator<<(ostream &os, Account &acc)
{
  os << "Nume:" << acc.getNume() << endl;
  os << "Prenume:" << acc.getPrenume() << endl;
  os << "Numarul contului:" << acc.getNrCont() << endl;
  os << "Suma introdusa:" << acc.getSuma() << endl;
  return os;
}

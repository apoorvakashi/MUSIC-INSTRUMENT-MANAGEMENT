#include <iostream>
// #include <fstream>
// #include <cstring>
// #include <charconv>
// #include <iomanip>
#include <string>

using namespace std;

// #include "delimbuffer.h"
// #include "bufferfile.h"
// #include "recfile.h"
// #include "dvd.h"

// int main() {
//   Dvd dvd;
//   DelimFieldBuffer::SetDefaultDelim('|');
//   DelimFieldBuffer Buff;
//   RecordFile<Dvd> DvdFile(Buff);

//   DvdFile.Open("dvd-list.txt", ios_base::in);

//   int recAddr = 149;
//   int result = DvdFile.Read(dvd, recAddr);
//   cout << "Record read from address " << result << "\n";
//   cout << dvd << "\n";
//   DvdFile.Close();
//   return 0;
// }

int main ()
{
  string str1, str2;
  cout << "Enter string 1: ";
  cin >> str1;
  cin.ignore('\n');
  cout << "Enter string 2: ";
  cin >> str2;

  cout << "String 1: " << str1 << " String 2: " << str2 << "\n";

  return 0;
}
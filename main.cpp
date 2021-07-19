#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <iomanip>
#include <stdexcept>

using namespace std;

#include "color.h"
#include "btreenode.h"
#include "btree.h"
#include "utils.h"
#include "iobuffer.h"
#include "instrument.h"
#include "index.h"
#include "bufferfile.h"
#include "recfile.h"
#include "indexfile.h"

#define RESET "\x1B[0m"

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAGENTA "\x1B[35m"
#define CYAN "\x1B[36m"

#define RED_BG "\x1B[41m"
#define GREEN_BG "\x1B[42m"
#define YELLOW_BG "\x1B[43m"
#define BLUE_BG "\x1B[44m"
#define MAGENTA_BG "\x1B[45m"
#define CYAN_BG "\x1B[46m"

#define BOLD "\x1B[1m"
#define ITALIC "\x1B[3m"
#define REVERSED "\x1B[7m"

void addNewInstrumentRecord(RecordFile<Instrument> &File, Instrument &instrument, BTree &tree)
{
  File.Open(ios_base::out | ios_base::app | ios_base::ate);
  cout << "\nEnter the Instrument details:\n\n";
  instrument.ReadFromStandardInput(tree);
  int recAddr = File.Write(instrument);
  cout << GREEN << "\nNew record written at address " << recAddr << "\n";
  File.Close();
  tree.insert(instrument.getID(), recAddr);
}

void displayAllInstrumentRecordsEntrySequenced(RecordFile<Instrument> &File, Instrument &instrument)
{
  File.Open(ios_base::in);
  cout << "\nInstrument Records - Based on entry-sequence:\n\n";
  instrument.PrintHeadings();
  while (true)
  {
    if (File.Read(instrument) == -1)
      break;
    if (instrument.ID[0] != '$')
      instrument.PrintRecord();
  }
  cout << BLUE << string(140, '-') << "\n"
       << RESET;
  File.Close();
}

void displayAllInstrumentRecordsSortedById(RecordFile<Instrument> &File, Instrument &instrument, BTree &tree)
{
  File.Open(ios_base::in);
  cout << "\nInstrument Records - Sorted by ID:\n\n";
  vector<pair<int, int>> nodes = tree.fetchAll();
  instrument.PrintHeadings();
  for (auto node : nodes)
  {
    if (File.Read(instrument, node.second) == -1)
      break;
    instrument.PrintRecord();
  }
  cout << BLUE << string(140, '-') << "\n"
       << RESET;
  File.Close();
}

int searchInstrumentRecord(int key, RecordFile<Instrument> &File, Instrument &instrument, BTree &tree)
{
  cout << BLUE << ITALIC << "\nSearching based on ID...\n\n"
       << RESET;
  int result = tree.search(key);
  if (result == -1)
    cout << GREEN << "\nRecord not found.\n"
         << RESET;
  else
  {
    File.Open(ios_base::in);
    File.Read(instrument, result);
    File.Close();
    cout << GREEN << "\nRecord found.\n"
         << RESET;
    cout << instrument << "\n";
  }
  return result;
}

int removeInstrumentRecord(int key, RecordFile<Instrument> &File, Instrument &Instrument, BTree &tree)
{
  int recAddr = searchInstrumentRecord(key, File, Instrument, tree);
  if (recAddr != -1)
  {
    cout << BLUE << ITALIC << "\nAre you sure you want to delete the record? [y/n]\n"
         << RESET;
    char ch;
    cin >> ch;

    if (ch == 'y' || ch == 'Y')
    {
      File.Open(ios_base::in | ios_base::out);
      File.MarkAsDeleted(recAddr);
      File.Close();
      tree.remove(key);
      cout << GREEN << "\nRecord deleted successfully.\n"
           << RESET;
    }
    else
      return -1;
  }
  return recAddr;
}

int modifyInstrumentRecord(int key, RecordFile<Instrument> &File, Instrument &instrument, BTree &tree)
{
  int recAddr = searchInstrumentRecord(key, File, instrument, tree);
  int result;
  if (recAddr != -1)
  {
    File.Open(ios_base::in | ios_base::out);
    File.MarkAsDeleted(recAddr);
    File.Close();
    tree.remove(key);

    instrument.ModifyRecord();

    File.Open(ios_base::out | ios_base::app | ios_base::ate);
    result = File.Write(instrument);
    cout << GREEN << "Modified record written at address " << result << "\n"
         << RESET;
    File.Close();
    tree.insert(instrument.getID(), result);
    return result;
  }
  return recAddr;
}

void treeTraversal(BTree &tree)
{
  vector<pair<int, int>> nodes = tree.fetchAll();
  for (auto node : nodes)
    cout << "Key = " << node.first << " Record Address = " << node.second << "\n";
}

void printMenu()
{
  cout << ITALIC;
  cout << YELLOW;
  cout << "\n"
       << string(40, '_') << "\n";
  cout << "\n1. Insert";
  cout << "\n2. Display (Entry-Sequenced)";
  cout << "\n3. Display (Sorted by ID)";
  cout << "\n4. Search by ID";
  cout << "\n5. Delete by ID";
  cout << "\n6. Modify by ID";
  cout << "\n7. Traverse the BTree";
  cout << "\n8. Exit the program";
  cout << "\n"
       << string(40, '_') << "\n";
  cout << RESET;
}

// Main function
int main()
{
  cout << setiosflags(ios_base::left);
  int choice, key, rewriteFlag = false;
  IOBuffer RecordBuff('|');
  IOBuffer IndexBuff('|');
  Instrument instrument;
  BTree tree(5);
  RecordFile<Instrument> InstrumentFile(RecordBuff, "instrument-list.txt");
  IndexFile BTreeIndexFile(IndexBuff, "instrument-index.txt");

  InstrumentFile.Create();
  BTreeIndexFile.Create();

  BTreeIndexFile.Open(ios_base::in);
  BTreeIndexFile.Read(tree);
  BTreeIndexFile.Close();

  do
  {
    printMenu();
    cout << "\nEnter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
      addNewInstrumentRecord(InstrumentFile, instrument, tree);
      rewriteFlag = true;
      break;

    case 2:
      displayAllInstrumentRecordsEntrySequenced(InstrumentFile, instrument);
      break;

    case 3:
      displayAllInstrumentRecordsSortedById(InstrumentFile, instrument, tree);
      break;

    case 4:
      cout << "\nEnter the key to be searched: ";
      cin >> key;
      searchInstrumentRecord(key, InstrumentFile, instrument, tree);
      break;

    case 5:
      cout << "\nEnter the key to be deleted: ";
      cin >> key;
      if (removeInstrumentRecord(key, InstrumentFile, instrument, tree) != -1)
        rewriteFlag = true;
      break;

    case 6:
      cout << "\nEnter the key to be modified: ";
      cin >> key;
      if (modifyInstrumentRecord(key, InstrumentFile, instrument, tree) != -1)
        rewriteFlag = true;
      break;

    case 7:
      treeTraversal(tree);
      break;

    case 8:
      cout << GREEN << "\nProgram terminated.\n"
           << RESET;
      break;

    default:
      cout << "\nInvalid choice. Please try again!\n";
    }
  } while (choice != 8);

  if (rewriteFlag)
  {
    cout << BLUE << ITALIC "\nRewriting Index File....\n"
         << RESET;
    BTreeIndexFile.Open(ios_base::out);
    BTreeIndexFile.Write(tree);
    BTreeIndexFile.Close();
    cout << GREEN << "\nSuccess!\n"
         << RESET;
  }

  return 0;
}
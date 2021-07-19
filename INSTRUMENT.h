class Instrument
{
public:
  string ID;
  string Type;
  string Description;
  string Brand;
  string Color;
  string Price;
  string Quantity;

  Instrument();
  void Clear();
  int getID();
  void ReadFromStandardInput(BTree &tree);
  void Pack(IOBuffer &buffer) const;
  int Unpack(IOBuffer &buffer);
  void PrintHeadings();
  void ModifyRecord();
  void PrintRecord();
};

Instrument::Instrument()
{
  Clear();
}

void Instrument::Clear()
{
  ID.clear();
  Type.clear();
  Description.clear();
  Brand.clear();
  Color.clear();
  Price.clear();
  Quantity.clear();
}

int Instrument::getID()
{
  return stoi(ID);
}

void Instrument::ReadFromStandardInput(BTree &tree)
{

  cout << CYAN;
  int valid, temp;
  do
  {
    cout << "Enter the Instrument ID: ";
    cin >> ID;
    cin.sync();
    valid = validateID(tree, ID);
    if (!valid)
      cout << "\nInvalid ID. Please try again.\n";
  } while (!valid);

  cout << "Enter the type of Instrument: ";
  cin >> ws;
  getline(cin, Type);

  cout << "Enter the description (Model number, Material): ";
  cin >> Description;
  cin.sync();

  cout << "Enter the Brand name : ";
  cin >> ws;
  getline(cin, Brand);

  cout << "Enter the Color : ";
  cin >> ws;
  getline(cin, Color);

  do
  {
    cout << "Enter the price: ";
    cin >> Price;
    cin.sync();
    valid = validateField('P', Price);
    if (!valid)
      cout << "\nPlease enter a valid price.\n";
  } while (!valid);

  do
  {
    cout << "Enter the quantity: ";
    cin >> Quantity;
    cin.sync();
    valid = validateField('Q', Quantity);
    if (!valid)
      cout << "\nPlease enter a valid number.\n";
  } while (!valid);
  cout << RESET;
}

void Instrument::Pack(IOBuffer &buffer) const
{
  buffer.Clear();
  buffer.Pack(ID);
  buffer.Pack(Type);
  buffer.Pack(Description);
  buffer.Pack(Brand);
  buffer.Pack(Color);
  buffer.Pack(Price);
  buffer.Pack(Quantity);
}

int Instrument::Unpack(IOBuffer &buffer)
{
  Clear();
  int numBytes;
  numBytes = buffer.Unpack(ID);
  if (numBytes == -1)
    return false;
  numBytes = buffer.Unpack(Type);
  if (numBytes == -1)
    return false;
  numBytes = buffer.Unpack(Description);
  if (numBytes == -1)
    return false;
  numBytes = buffer.Unpack(Brand);
  if (numBytes == -1)
    return false;
  numBytes = buffer.Unpack(Color);
  if (numBytes == -1)
    return false;
  numBytes = buffer.Unpack(Price);
  if (numBytes == -1)
    return false;
  numBytes = buffer.Unpack(Quantity);
  if (numBytes == -1)
    return false;
  return true;
}

void Instrument::PrintHeadings()
{
  cout << BLUE;
  cout << string(140, '-') << "\n";
  cout << setw(5) << "ID" << setw(50) << "TYPE" << setw(20)
       << "DESCRIPTION" << setw(30) << "BRAND" << setw(6) << "COLOR"
       << setw(10) << "QUANTITY" << setw(10) << "PRICE"
       << "\n";
  cout << string(140, '-') << "\n";
  cout << RESET;
}

void Instrument::PrintRecord()
{
  cout << setw(5) << ID << setw(50) << Type << setw(20)
       << Description << setw(30) << Brand << setw(6) << Color
       << setw(10) << Quantity << setw(10) << Price << "\n";
}

ostream &operator<<(ostream &stream, Instrument d)
{
  stream << "\nID: " << d.ID << "\nType: " << d.Type
         << "\nDescriptionuage: " << d.Description << "\nBrand: "
         << d.Brand << "\nColor: " << d.Color << "\nQuantity: "
         << d.Quantity << "\nPrice: " << d.Price << "\n";
  return stream;
}

void Instrument::ModifyRecord()
{
  string temp;
  int choice;
  do
  {
    cout << *this << "\n";
    cout << "Which field would you like to modify?\n";
    cout << "1. ID\t2. Type\t3. Description\t4. Brand\t5. Color\t6. Quantity\t7. Price\t8. Back to main menu";
    cout << "\nEnter the field no. : ";
    cin >> choice;
    if (choice > 0 && choice < 8)
    {
      cout << "\nEnter the new value: ";
      cin >> ws;
      getline(cin, temp);
    }

    switch (choice)
    {
    case 1:
      ID = temp;
      break;
    case 2:
      Type = temp;
      break;
    case 3:
      Description = temp;
      break;
    case 4:
      Brand = temp;
      break;
    case 5:
      Color = temp;
      break;
    case 6:
      Quantity = temp;
      break;
    case 7:
      Price = temp;
      break;
    case 8:
      cout << "\nApplying modifications...";
      break;
    default:
      cout << "Invalid choice. Please enter again.";
    }

  } while (choice != 8);
}
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
  void readID(BTree tree);
  void readType();
  void readDescription();
  void readBrand();
  void readColor();
  void readPrice();
  void readQuantity();
  void ReadFromStandardInput(BTree tree);
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

void Instrument::readID(BTree tree)
{
  int valid;
  do
  {
    cout << "Enter the ID: ";
    cin >> ID;
    cin.sync();
    valid = validateID(tree, ID);
  } while (!valid);
}

void Instrument::readType()
{
  cout << "Enter the Type: ";
  cin >> ws;
  getline(cin, Type);
}

void Instrument::readBrand()
{
  cout << "Enter the Brand name : ";
  cin >> ws;
  getline(cin, Brand);
}

void Instrument::readColor()
{
  cout << "Enter the Color : ";
  cin >> ws;
  getline(cin, Color);
}

void Instrument::readDescription()
{
  cout << "Enter the description (Model number, Material): ";
  cin >> ws;
  getline(cin, Description);
}

void Instrument::readPrice()
{
  int valid;
  do
  {
    cout << "Enter the price: ";
    cin >> Price;
    cin.sync();
    valid = validateField('P', Price);
    if (!valid)
      cout << "\nPlease enter a valid price.\n";
  } while (!valid);
}

void Instrument::readQuantity()
{
  int valid;
  do
  {
    cout << "Enter the quantity: ";
    cin >> Quantity;
    cin.sync();
    valid = validateField('Q', Quantity);
    if (!valid)
      cout << "\nPlease enter a valid number.\n";
  } while (!valid);
}
void Instrument::ReadFromStandardInput(BTree tree)
{
  cout << MAGENTA;
  readID(tree);
  readType();
  readDescription();
  readBrand();
  readColor();
  readPrice();
  readQuantity();
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
  cout << YELLOW;
  cout << string(140, '-') << "\n";
  cout << setw(5) << "ID" << setw(25) << "TYPE" << setw(50)
       << "DESCRIPTION" << setw(20) << "BRAND" << setw(15) << "COLOR"
       << setw(15) << "QUANTITY" << setw(10) << "PRICE"
       << "\n";
  cout << string(140, '-') << "\n";
  cout << RESET;
}

void Instrument::PrintRecord()
{
  cout << setw(5) << ID << setw(25) << Type << setw(50)
       << Description << setw(20) << Brand << setw(10) << Color
       << setw(15) << Quantity << setw(10) << Price << "\n";
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
    cout << "Which field would you like to modify?\n\n";
    cout << YELLOW << "1. ID\t2. Type\t3. Description\t4. Brand\t5. Color\t6. Quantity\t7. Price\t8. Back to main menu\n"
         << RESET;
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
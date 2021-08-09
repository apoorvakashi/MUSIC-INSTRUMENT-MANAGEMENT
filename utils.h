int validateNumStr(string value)
{
  int num;
  try
  {
    num = stoi(value);
  }
  catch (invalid_argument)
  {
    return -1;
  }
  return num;
}

int validateID(BTree tree, string ID)
{
  int id = validateNumStr(ID);
  if (id <= 0)
  {
    cout << RED << "\nPlease enter a positive integer for ID.\n\n"
         << CYAN;
    return false;
  }
  if (tree.search(id) == -1)
    return true;
  else
  {
    cout << RED << "\nThis ID already exists.\n\n"
         << CYAN;
    return false;
  }
}

int validateField(const char field, string value)
{
  int num = validateNumStr(value);
  int valid;
  switch (field)
  {
  case 'P':
    valid = (num >= 200 && num <= 30000);
    break;
  case 'Q':
    valid = (num >= 0 && num <= 10);
    break;
  }
  return valid;
}
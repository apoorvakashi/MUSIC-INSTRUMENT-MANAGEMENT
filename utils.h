int validateNumStr(string value) {
  int num;
  try {
    num = stoi(value);
  } catch (invalid_argument) {
    return -1;
  }
  return num;
}


int validateID(BTree & tree, string ID) {
  int id = validateNumStr(ID);
  return (id > 0 && (tree.search(id) == -1));
}

int validateField(const char field, string value) {
  int num = validateNumStr(value);
  int valid;
  switch (field) {
    case 'P':
    case 'Q': valid = (num > 0); break;
    case 'Y': valid = (num > 0 && value.size() == 4); break;
  }
  return valid;
}
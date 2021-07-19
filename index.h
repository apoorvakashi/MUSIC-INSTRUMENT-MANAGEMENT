class Index {
  public:
    string key;
    string addr;

    Index();
    void Clear();
    void setIndex(int _key, int _addr);
    int getKey();
    int getAddr();
    void Pack(IOBuffer & buffer) const;
    int Unpack(IOBuffer & buffer);
};

Index::Index() {
  Clear();
}

void Index::Clear() {
  key.clear();
  addr.clear();
}

void Index::setIndex(int _key, int _addr) {
  key = to_string(_key);
  addr = to_string(_addr);
}

int Index::getKey() {
  return stoi(key);
}

int Index::getAddr() {
  return stoi(addr);
}

void Index::Pack(IOBuffer & buffer) const {
  buffer.Clear();
  buffer.Pack(key);
  buffer.Pack(addr);
}

int Index::Unpack(IOBuffer & buffer) {
  Clear();
  int numBytes;
  numBytes = buffer.Unpack(key);
  if (numBytes == -1)
    return false;
  numBytes = buffer.Unpack(addr);
  if (numBytes == -1)
    return false;
  return true;
}
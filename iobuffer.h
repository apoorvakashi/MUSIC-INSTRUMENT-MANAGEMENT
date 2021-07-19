class IOBuffer {
  public:
    IOBuffer(char delim = '|');
    IOBuffer(const IOBuffer & buffer);
    void Init(char delim);
    void Clear();
    void Pack(const string field);
    int Unpack(string & field);
    int Read(istream & stream);
    int Write(ostream & stream) const;
    int DRead(istream & stream, int recRef);
    int DWrite(ostream & stream, int recRef) const;
  
  protected:
    char Delim;
    string Buffer;
    int BufferSize;
    int NextByte;
};

IOBuffer::IOBuffer(const IOBuffer & buffer) {
  Init(buffer.Delim);
}

IOBuffer::IOBuffer(char delim) {
  Init(delim);
}

void IOBuffer::Init(char delim) {
  Delim = delim;
  Clear();
}

void IOBuffer::Clear() {
  NextByte = 0;
  Buffer.clear();
  BufferSize = 0;
}

void IOBuffer::Pack(const string field) {
  Buffer.append(field);
  Buffer.append(1, Delim);
}

int IOBuffer::Unpack(string & field) {
  if (NextByte > BufferSize)
    return -1;
  
  int len = -1;
  int start = NextByte;
  for (int i = start; i < BufferSize; i++)
    if (Buffer[i] == Delim) {
      len = i - start;
      break;
    }
  
  if (len == -1) // delimiter not found
    return -1;
  
  NextByte += (len + 1);
  field = Buffer.substr(start, len);
  return len;
}

int IOBuffer::Read(istream & stream) {
  if (stream.eof())
    return -1;
  int recAddr = stream.tellg();
  Clear();
  getline(stream, Buffer, '#');
  if (!stream.good()) {
    stream.clear();
    return -1;
  }
  BufferSize = Buffer.size();
  return recAddr;
}

int IOBuffer::Write(ostream & stream) const {
  int recAddr = stream.tellp();
  stream << Buffer << '#';
  if (!stream.good())
    return -1;
  return recAddr;
}

int IOBuffer::DRead(istream & stream, int recRef) {
  stream.seekg(recRef, ios_base::beg);
  if (stream.tellg() != recRef)
    return -1;
  return Read(stream);
}

int IOBuffer::DWrite(ostream & stream, int recRef) const {
  stream.seekp(recRef, ios_base::beg);
  if (stream.tellp() != recRef)
    return -1;
  return Write(stream);
}
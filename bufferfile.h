class BufferFile {
  public:
    BufferFile(IOBuffer & buffer, const string filename);
    void Create();
    int Open(ios_base::openmode mode);
    void Close();
    int Read(int recAddr = -1);
    int Write(int recAddr = -1);
    void MarkAsDeleted(int recAddr);
  
  protected:
    IOBuffer & Buffer;
    string Filename;
    fstream File;
};

BufferFile::BufferFile(IOBuffer & buffer, const string filename): Buffer(buffer) {
  Filename = filename;
}

void BufferFile::Create() {
  File.open(Filename, ios_base::in);
  if (!File) {
    cout << "Creating file " << Filename << "...\n";
    File.open(Filename, ios_base::out);
    cout << "Done!\n";
  }
  File.close();
}

int BufferFile::Open(ios_base::openmode mode) {
  File.open(Filename, mode);
  return File.good();
}

void BufferFile::Close() {
  File.close();
}

int BufferFile::Read(int recAddr) {
  if (recAddr == -1)
    return Buffer.Read(File);
  else
    return Buffer.DRead(File, recAddr);
}

int BufferFile::Write(int recAddr) {
  if (recAddr == -1)
    return Buffer.Write(File);
  else
    return Buffer.DWrite(File, recAddr);
}

void BufferFile::MarkAsDeleted(int recAddr) {
  File.seekp(recAddr, ios_base::beg);
  File.put('$');
}
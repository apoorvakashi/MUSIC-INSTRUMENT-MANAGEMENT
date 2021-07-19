template <class RecType>
class RecordFile: public BufferFile {
  public:
    RecordFile(IOBuffer & buffer, const string filename): BufferFile(buffer, filename) {}
    int Read(RecType & record, int recAddr = -1);
    int Write(const RecType & record, int recAddr = -1);
};

template <class RecType>
int RecordFile<RecType>::Read(RecType & record, int recAddr) {
  int readAddr, result;
  readAddr = BufferFile::Read(recAddr);
  if (readAddr == -1) return -1;
  result = record.Unpack(Buffer);
  if (!result) return -1;
  return readAddr;
}

template <class RecType>
int RecordFile<RecType>::Write(const RecType & record, int recAddr) {
  record.Pack(Buffer);
  return BufferFile::Write(recAddr);
}
class IndexFile: public BufferFile {
  public:
    IndexFile(IOBuffer & buffer, const string filename): BufferFile(buffer, filename) {}
    void Read(BTree & tree);
    void Write(BTree & tree);
  
  protected:
    Index index;
};

void IndexFile::Read(BTree & tree) {
  int readAddr, result;
  while (true) {
    readAddr = BufferFile::Read();
    if (readAddr == -1) break;
    result = index.Unpack(Buffer);
    if (!result) {
      cout << "Some error occurred while unpacking index file\n";
      break;
    }
    tree.insert(index.getKey(), index.getAddr());
  }
  
}

void IndexFile::Write(BTree & tree) {
  vector<pair<int, int>> nodes = tree.fetchAll();
  int result;

  for (auto node: nodes) {
    index.setIndex(node.first, node.second);
    index.Pack(Buffer);
    result = BufferFile::Write();
  }
}
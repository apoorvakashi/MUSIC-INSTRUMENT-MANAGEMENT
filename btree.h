class BTree {
  BTNode * root;    // Pointer to root node
  int t;            // Minimum degree

  public:
  // Constructor
  BTree(int _t);

  // A function to traverse the tree
  void traverse();

  vector<pair<int, int>> fetchAll();

  // A function to search a key in this tree
  // BTNode * search(string key);
  int search(int key);

  // A function that inserts a new key in this BTree
  // void insert(string key);
  void insert(int key, int recAddr);

  // A function that removes a key in this BTree
  void remove(int key);
};

BTree::BTree(int _t) {
  root = NULL;
  t = _t;
}

void BTree::traverse() {
  if (root != NULL)
    root->traverse();
}

vector<pair<int, int>> BTree::fetchAll() {
  vector<pair<int, int>> nodes;
  if (root != NULL)
    nodes = root->fetchAll();
  return nodes;
}

// BTNode * BTree::search(string key) {
int BTree::search(int key) {
  return (root == NULL) ? -1 : root->search(key);
}

// void BTree::insert(string key) {
void BTree::insert(int key, int recAddr) {
  // If tree is empty
  if (root == NULL) {
    root = new BTNode(t, true);     // Allocate memory for root
    root->Keys[0] = key;            // Insert key
    root->RecAddrs[0] = recAddr;
    root->n = 1;                    // Update number of keys in root
  }

  // If tree is not empty
  else {
    // If root is full, then tree grows in height
    if (root->n == (2 * t - 1)) {
      // Allocate memory for new root
      BTNode * s = new BTNode(t, false);

      // Make old root as child of new root
      s->C[0] = root;

      // Split the old root and move 1 key to the new root
      s->splitChild(0, root);

      // New root has two children now
      // Decide which of the two children is going to have new key
      int i = 0;
      // if (s->Keys[0].compare(key) < 0)
      if (s->Keys[0] < key)
        i++;
      s->C[i]->insertNonFull(key, recAddr);

      // Change root
      root = s;
    }

    // If root is not full
    else
      root->insertNonFull(key, recAddr);
  }
}

void BTree::remove(int key) {
  if (root == NULL) {
    cout << "The tree is empty\n";
  }

  // Call th remove function for root
  root->remove(key);

  // If the root node has 0 keys, make its first child as the new root
  // If it has a child, set root as NULL
  if (root->n == 0) {
    BTNode * temp = root;
    if (root->leaf)
      root = NULL;
    else
      root = root->C[0];

    delete temp;
  }
}
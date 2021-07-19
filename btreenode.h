class BTNode {
  int * Keys;    // An array of keys
  int * RecAddrs;   // An array of record addresses
  int t;            // Minimum degree (defines the range for number of keys)
  BTNode ** C;       // An array of child pointers
  int n;            // Current number of keys
  bool leaf;        // TRUE when node is leaf, otherwise FALSE

  public:
  // Constructor
  BTNode(int _t, bool _leaf);
  ~BTNode();

  // A function to traverse all nodes in a subtree rooted with this node
  void traverse();

  vector<pair<int, int>> fetchAll();

  // A function to search a key in subtree rooted with this node
  int search(int key);

  // A function that returns the index of the first key >= key
  int findKey(int key);

  // A utility function to insert a new key in the subtree rooted with this node.
  // The assumption is that the node must be non-full when this function is called.
  void insertNonFull(int key, int recAddr);

  // A utility function to split the child y of this node.
  // i is index of y in child array C[].
  // The child y must be full when this function is called.
  void splitChild(int i, BTNode * y);

  // A function to remove key in subtree rooted with this node.
  void remove(int key);

  // A function to remove the key present in idx-th position in this node which is a leaf.
  void removeFromLeaf(int idx);

  // A function to remove the key present in idx-th position in this node which is not a leaf.
  void removeFromNonLeaf(int idx);

  // A function to get the predecessor of the key where the key
  // is present in the idx-th position in this node
  pair<int, int> getPred(int idx);

  // A function to get the successor of the key where the key
  // is present in the idx-th position in this node
  pair<int, int> getSucc(int idx);

  // A function to fill up the child node present in the idx-th
  // position in the array C[] if that child has less than t - 1 keys
  void fill(int idx);

  // A function to borrow a key from the C[idx - 1]-th node and place it in C[idx]-th node
  void borrowFromPrev(int idx);

  // A function to borrow a key from the C[idx + 1]-th node and place it in C[idx]-th node
  void borrowFromNext(int idx);

  // A function to merge idx-th child of the node with (idx + 1)-th child of the node
  void merge(int idx);

  // Make BTree friend of this so that we can access private members of this class in BTree functions.
  friend class BTree;
};

BTNode::BTNode(int _t, bool _leaf) {
  // Copy the given minimum degree and leaf property
  t = _t;
  leaf = _leaf;

  // Allocate memory for maximum number of possible keys and child pointers
  Keys = new int[2 * t - 1];
  RecAddrs = new int[2 * t - 1];
  C = new BTNode*[2 * t];

  // Initialize number of keys as 0
  n = 0;
}

BTNode::~BTNode() {
  delete[] Keys;
  delete[] RecAddrs;
  delete[] C;
}

void BTNode::traverse() {
  // There are n keys and n + 1 children
  // Traverse through n keys and first n children
  int i;
  for (i = 0; i < n; i++) {
    // If this is not lead, then before printing Keys[i]
    // traverse the subtree rooted with child C[i]
    if (leaf == false)
      C[i]->traverse();
    cout << "Key = " << Keys[i] << " Record Address = " << RecAddrs[i] << "\n";
  }

  // Print the subtree rooted with last child
  if (leaf == false)
    C[i]->traverse();
}

vector<pair<int, int>> BTNode::fetchAll() {
  vector<pair<int, int>> nodes;
  int i;
  for (i = 0; i < n; i++) {
    vector<pair<int, int>> temp;
    if (!leaf)
      temp = C[i]->fetchAll();
    
    for (auto it: temp)
      nodes.push_back(it);
    
    nodes.push_back(make_pair(Keys[i], RecAddrs[i]));
  }
  
  vector<pair<int, int>> temp;
  if (!leaf)
    temp = C[i]->fetchAll();
  
  for (auto it: temp)
    nodes.push_back(it);
  
  return nodes;
}

int BTNode::search(int key) {
  // Find the first key greater than or equal to key
  int i = 0;
  while (i < n && key > Keys[i])
    i++;
  
  // If the found key is equal to key, return this node
  if (key == Keys[i])
    return RecAddrs[i];
  
  // If the key is not found and this is a leaf node
  if (leaf == true)
    return -1;
  
  // Go to the appropriate child
  return C[i]->search(key);
}

// void BTNode::insertNonFull(string key) {
void BTNode::insertNonFull(int key, int recAddr) {
  // Initialize index as index of rightmost element
  int i = n - 1;

  // If this is a leaf node
  if (leaf == true) {
    // The following loop does two things
    // 1. Finds the location of new key to be inserted
    // 2. Moves all greater keys to one place ahead
    while (i >= 0 && Keys[i] > key) {
      Keys[i + 1] = Keys[i];
      RecAddrs[i + 1] = RecAddrs[i];
      i--;
    }

    // Insert new key at found location
    Keys[i + 1] = key;
    RecAddrs[i + 1] = recAddr;
    n += 1;
  }

  // If this node is not a leaf
  else {
    // Find the child which is going to have the new key
    while (i >= 0 && Keys[i] > key)
      i--;
    
    // See if the found child is full
    if (C[i + 1]->n == (2 * t - 1)) {
      // If the child is full, then split it
      splitChild(i + 1, C[i + 1]);

      // After split, the middle key of C[i] goes up and C[i] is split into two.
      // See which of the two is going to have the new key
      if (Keys[i + 1] < key)
        i++;
    }
    C[i + 1]->insertNonFull(key, recAddr);
  }
}

void BTNode::splitChild(int i, BTNode * y) {
  // Create a new node which is going to store (t - 1) keys of y
  BTNode * z = new BTNode(y->t, y->leaf);
  z->n = t - 1;

  // Copy the last (t - 1) keys of y to z
  for (int j = 0; j < t - 1; j++) {
    z->Keys[j] = y->Keys[j + t];
    z->RecAddrs[j] = y->RecAddrs[j + t];
  }
  
  // Copy the last t children of y to z
  if (y->leaf == false) {
    for (int j = 0; j < t; j++)
      z->C[j] = y->C[j + t];
  }

  // Reduce the number of keys in y
  y->n = t - 1;

  // Since this node is going to have a new child, create space of new child
  for (int j = n; j >= i + 1; j--)
    C[j + 1] = C[j];

  // Link the new child to this node
  C[i + 1] = z;

  // A key of y will move to this node.
  // Find location of new key and move all greater keys one space ahead.
  for (int j = n - 1; j >= i; j--) {
    Keys[j + 1] = Keys[j];
    RecAddrs[j + 1] = RecAddrs[j];
  }
  
  // Copy the middle key of y to this node
  Keys[i] = y->Keys[t - 1];
  RecAddrs[i] = y->RecAddrs[t - 1];

  // Increment count of keys in this nodes
  n += 1;
}

int BTNode::findKey(int key) {
  int idx = 0;
  while (idx < n && Keys[idx] < key)
    idx++;
  return idx;
}

void BTNode::remove(int key) {
  int idx = findKey(key);

  // The key to be removed is present in this node
  if (idx < n && Keys[idx] == key) {
    // If the node is a leaf node, removeFromLeaf is called
    if (leaf)
      removeFromLeaf(idx);
    // otherwise, removeFromNonLeaf is called
    else
      removeFromNonLeaf(idx);
  }
  else {
    // If this node is a leaf node, then the key is not present in tree
    if (leaf) {
      cout << "The key " << key << " does not exist in the tree.\n";
      return;
    }

    // The key to be removed is present in the sub-tree rooted with this node
    // The flag indicates whether the key is present in the sub-tree rooted
    // with the last child of this node
    bool flag = ((idx == n) ? true : false);

    // If the child where the key is supposed to exist has less that t keys,
    // we fill that child
    if (C[idx]->n < t)
      fill(idx);
    
    // If the last child has been merged, it must have merged with the previous
    // child and so we recurse on the (idx - 1)-th child. Else, we recurse on the
    // idx-th child which now has atleast t keys
    if (flag && idx > n)
      C[idx - 1]->remove(key);
    else
      C[idx]->remove(key);
  }
}

void BTNode::removeFromLeaf(int idx) {
  for (int i = idx + 1; i < n; ++i) {
    Keys[i - 1] = Keys[i];
    RecAddrs[i - 1] = RecAddrs[i];
  }
  n--;
}

void BTNode::removeFromNonLeaf(int idx) {
  int key = Keys[idx];
  // If the child that precedes k (C[idx]) has atleast t keys,
  // find the predecessor 'pred' of k in the subtree rooted at
  // C[idx]. Replace k by pred. Recursively delete pred
  // in C[idx]
  if (C[idx]->n >= t) {
    pair<int, int> pred = getPred(idx);
    Keys[idx] = pred.first;
    RecAddrs[idx] = pred.second;
    C[idx]->remove(pred.first);
  }

  // If the child C[idx] has less that t keys, examine C[idx+1].
  // If C[idx+1] has atleast t keys, find the successor 'succ' of k in
  // the subtree rooted at C[idx+1]
  // Replace k by succ
  // Recursively delete succ in C[idx+1]
  else if (C[idx + 1]->n >= t) {
    pair<int, int> succ = getSucc(idx);
    Keys[idx] = succ.first;
    RecAddrs[idx] = succ.second;
    C[idx + 1]->remove(succ.first);
  }

  // If both C[idx] and C[idx+1] has less that t keys,
  // merge k and all of C[idx+1] into C[idx]
  // Now C[idx] contains 2t-1 keys
  // Free C[idx+1] and recursively delete k from C[idx]
  else {
    merge(idx);
    C[idx]->remove(key);
  }
}

pair<int, int> BTNode::getPred(int idx) {
  // Keep moving to the right most node until we reach a leaf
  BTNode * cur = C[idx];
  while (!cur->leaf)
    cur = cur->C[cur->n];
  
  // Return the last key of the leaf
  pair<int, int> p = make_pair(cur->Keys[cur->n - 1], cur->RecAddrs[cur->n - 1]);
  return p;
}

pair<int, int> BTNode::getSucc(int idx) {
  // Keep moving the left most node starting from C[idx+1] until we reach a leaf
  BTNode * cur = C[idx + 1];
  while (!cur->leaf)
    cur = cur->C[0];
  
  // Return the first key of the leaf
  pair<int, int> p = make_pair(cur->Keys[0], cur->RecAddrs[0]);
  return p;
}

void BTNode::fill(int idx) {
  // If the previous child(C[idx-1]) has more than t-1 keys, borrow a key
  // from that child
  if (idx != 0 && C[idx - 1]->n >= t)
    borrowFromPrev(idx);
  
  // If the next child(C[idx+1]) has more than t-1 keys, borrow a key
  // from that child
  else if (idx != n && C[idx + 1]->n >= t)
    borrowFromNext(idx);
  
  // Merge C[idx] with its sibling
  // If C[idx] is the last child, merge it with with its previous sibling
  // Otherwise merge it with its next sibling
  else {
    if (idx != n)
      merge(idx);
    else
      merge(idx - 1);
  }
}

void BTNode::borrowFromPrev(int idx) {
  BTNode * child = C[idx];
  BTNode * sibling = C[idx - 1];

  // The last key from C[idx-1] goes up to the parent and Keys[idx-1]
  // from parent is inserted as the first key in C[idx]. Thus, the sibling
  // loses one key and child gains one key

  // Moving all keys in C[idx] one step ahead
  for (int i = child->n - 1; i >= 0; --i) {
    child->Keys[i + 1] = child->Keys[i];
    child->RecAddrs[i + 1] = child->RecAddrs[i];
  }

  // If C[idx] is not a leaf, move all its child pointers one step ahead
  if (!child->leaf) {
    for (int i = child->n; i >= 0; --i)
      child->C[i + 1] = child->C[i];
  }

  // Setting child's first key equal to Keys[idx-1] from the current node
  child->Keys[0] = Keys[idx - 1];
  child->RecAddrs[0] = RecAddrs[idx - 1];

  // Moving sibling's last child as C[idx]'s first child
  if (!child->leaf)
    child->C[0] = sibling->C[sibling->n];
  
  // Moving the key from the sibling to the parent
  // This reduces the number of keys in the sibling
  Keys[idx - 1] = sibling->Keys[sibling->n - 1];
  RecAddrs[idx - 1] = sibling->RecAddrs[sibling->n - 1];

  child->n += 1;
  sibling->n -= 1;
}

void BTNode::borrowFromNext(int idx) {
  BTNode * child = C[idx];
  BTNode * sibling = C[idx + 1];

  // Keys[idx] is inserted as the last key in C[idx]
  child->Keys[child->n] = Keys[idx];
  child->RecAddrs[child->n] = RecAddrs[idx];

  // Sibling's first child is inserted as the last child into C[idx]
  if (!child->leaf)
    child->C[child->n + 1] = sibling->C[0];
  
  // The first key from sibling is inserted into Keys[idx]
  Keys[idx] = sibling->Keys[0];
  RecAddrs[idx] = sibling->RecAddrs[0];

  // Moving all keys in sibling one step behind
  for (int i = 1; i < sibling->n; ++i) {
    sibling->Keys[i - 1] = sibling->Keys[i];
    sibling->RecAddrs[i - 1] = sibling->RecAddrs[i];
  }

  // Moving the child pointers one step behind
  if (!sibling->leaf) {
    for (int i = 1; i <= sibling->n; ++i)
      sibling->C[i - 1] = sibling->C[i];
  }

  // Increasing and decreasing the key count of C[idx] and C[idx+1] respectively
  child->n += 1;
  sibling->n -= 1;
}

void BTNode::merge(int idx) {
  BTNode * child = C[idx];
  BTNode * sibling = C[idx + 1];

  // Pulling a key from the current node and inserting it into (t-1)th position of C[idx]
  child->Keys[t - 1] = Keys[idx];
  child->RecAddrs[t - 1] = RecAddrs[idx];

  // Copying the keys from C[idx+1] to C[idx] at the end
  for (int i = 0; i < sibling->n; ++i) {
    child->Keys[i + t] = sibling->Keys[i];
    child->RecAddrs[i + t] = sibling->RecAddrs[i];
  }

  // Copying the child pointers from C[idx+1] to C[idx]
  if (!child->leaf) {
    for (int i = 0; i <= sibling->n; ++i)
      child->C[i + t] = sibling->C[i];
  }

  // Moving all keys after idx in the current node one step before -
  // to fill the gap created by moving keys[idx] to C[idx]
  for (int i = idx + 1; i < n; ++i) {
    Keys[i - 1] = Keys[i];
    RecAddrs[i - 1] = RecAddrs[i];
  }

  // Moving the child pointers after (idx+1) in the current node one step before
  for (int i = idx + 2; i <= n; ++i)
    C[i - 1] = C[i];
  
  // Updating the key count of child and the current node
  child->n += (sibling->n + 1);
  n--;

  delete sibling;
}
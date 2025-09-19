#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename KeyT, typename ValT>
class HashMap {
 private:
  struct ChainNode {
    const KeyT key;
    ValT value;
    ChainNode* next;

    ChainNode(KeyT key, ValT value) : key(key), value(value), next(nullptr) {
    }

    ChainNode(KeyT key, ValT value, ChainNode* next)
        : key(key), value(value), next(next) {
    }
  };

  ChainNode** data;
  size_t sz;
  size_t capacity;

  // Utility members for begin/next
  ChainNode* curr;
  size_t curr_idx;

 public:
  /**
   * Creates an empty `HashMap` with 10 buckets.
   */
  HashMap() {
    // TODO_STUDENT
    this->data = new ChainNode*[10];
    for (int i = 0; i < 10; i++) {
      this->data[i] = nullptr;
    }
    this->sz = 0;
    this->capacity = 10;
  }

  /**
   * Creates an empty `HashMap` with `capacity` buckets.
   */
  HashMap(size_t capacity) {
    // TODO_STUDENT
    this->capacity = capacity;
    this->data = new ChainNode*[capacity];
    for (int i = 0; i < capacity; i++) {
      this->data[i] = nullptr;
    }
    this->sz = 0;
  }

  /**
   * Checks if the `HashMap` is empty. Runs in O(1).
   */
  bool empty() const {
    // TODO_STUDENT
    return (this->sz == 0);
  }

  /**
   * Returns the number of mappings in the `HashMap`. Runs in O(1).
   */
  size_t size() const {
    // TODO_STUDENT
    return this->sz;
  }

  /**
   * Adds the mapping `{key -> value}` to the `HashMap`. If the key already
   * exists, does not update the mapping (like the C++ STL map).
   *
   * Uses the hash value of the key to determine the location in the
   * underlying hash table. Creates exactly one new node; resizes by doubling
   * when the load factor exceeds 1.5.
   *
   * On resize, doesn't create new nodes, but rearranges existing ones.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  void insert(KeyT key, ValT value) {
    // TODO_STUDENT
    int bucketNum = hash<KeyT>{}(key) % this->capacity;
    ChainNode* hashcurr = this->data[bucketNum];
    bool noexists = true;
    while (hashcurr != nullptr) {
      if (key == hashcurr->key) {
        noexists = false;
      }
      hashcurr = hashcurr->next;
    }
    if (noexists) {
      this->sz++;
      this->maybeResize();
      bucketNum = hash<KeyT>{}(key) % this->capacity;
      ChainNode* newNode = new ChainNode(key, value, this->data[bucketNum]);
      this->data[bucketNum] = newNode;
    }
  }

  void maybeResize() {  // helper resize function
    if ((double)this->sz / (double)this->capacity <= 1.5) {
      return;
    }
    int a = this->capacity;
    int newCapacity = this->capacity *= 2;
    ChainNode** newData = new ChainNode*[newCapacity];
    for (int i = 0; i < newCapacity; i++) {
      newData[i] = nullptr;
    }
    for (int i = 0; i < a; i++) {
      ChainNode* hashthis = this->data[i];
      ChainNode* hashnew = nullptr;
      ChainNode* tmp = nullptr;
      while(hashthis!=nullptr){
        int a = hash<KeyT>{}(hashthis->key) % newCapacity;
        if(newData[a] != nullptr){
          //tmp = newData[a]->next;
          tmp = newData[a];
        } else {
          tmp = nullptr;
        }
        newData[a] = hashthis;
        if(newData[a] != nullptr){
          hashthis = newData[a]->next;
          newData[a]->next = tmp;
        }
      }
    }
    this->capacity = newCapacity;
    delete[] this->data;
    this->data = newData;
  }

  /**
   * Return a reference to the value stored for `key` in the map.
   *
   * If key is not present in the map, throw `out_of_range` exception.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT& at(const KeyT& key) const {
    // TODO_STUDENT
    if (this->sz == 0) {
      throw out_of_range("ts dont exist: at function because sz 0");
    }
    int numBucket = hash<KeyT>{}(key) % this->capacity;
    ChainNode* hashcurr = this->data[numBucket];
    /*ChainNode* t1 = this->data[0];
      ChainNode* t2 = this->data[1];
      ChainNode* t3 = this->data[2];
      ChainNode* t4 = this->data[3];
      ChainNode* t5 = this->data[4];
      if(this->capacity == 10){
      ChainNode* t6 = this->data[5];
      ChainNode* t7 = this->data[6];
      ChainNode* t8 = this->data[7];
      ChainNode* t9 = this->data[8];
      }*/
    while (hashcurr != nullptr) {
      if (hashcurr->key == key) {
        return hashcurr->value;
      }
      hashcurr = hashcurr->next;
    }
    throw out_of_range("ts dont exist: at function couldn't find it");
  }

  /**
   * Returns `true` if the key is present in the map, and false otherwise.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  bool contains(const KeyT& key) const {
    int numBucket = hash<KeyT>{}(key) % this->capacity;
    ChainNode* hashcurr = this->data[numBucket];
    while (hashcurr != nullptr) {
      if (hashcurr->key == key) {
        return true;
      }
      hashcurr = hashcurr->next;
    }
    return false;
  }

  /**
   * Empties the `HashMap`, freeing all nodes. The bucket array may be left
   * alone.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  void clear() {
    // TODO_STUDENT
    for (int i = 0; i < this->capacity; i++) {
      ChainNode* hashcurr = this->data[i];
      ChainNode* hashnext = nullptr;
      while (hashcurr != nullptr) {
        if (hashcurr->next == nullptr) {
          delete hashcurr;
          hashcurr = nullptr;
        } else {
          hashnext = hashcurr->next;
          delete hashcurr;
          hashcurr = hashnext;
        }
      }
      hashnext = nullptr;
    }
    for (int i = 0; i < this->capacity; i++) {
      this->data[i] = nullptr;
    }
    this->sz = 0;
  }

  /**
   * Destructor, cleans up the `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  ~HashMap() {
    // TODO_STUDENT
    this->clear();
    delete[] this->data;
  }

  /**
   * Removes the mapping for the given key from the `HashMap`, and returns the
   * value.
   *
   * Throws `out_of_range` if the key is not present in the map. Creates no new
   * nodes, and does not update the key or value of any existing nodes.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT erase(const KeyT& key) {
    // TODO_STUDENT
    int a = hash<KeyT>{}(key);
    if(a<0){
      a*=-1;
    }
    ChainNode* curr = this->data[a%this->capacity];
    ChainNode* prev = nullptr;
    ValT val;
    bool notfound = true;
    while (curr != nullptr) {
      if (prev != nullptr && curr->key == key && curr->next != nullptr) { //middle
        prev->next = curr->next;
        val = curr->value;
        notfound = false;
        delete curr;
        curr = nullptr;
      } else if (prev != nullptr && curr->key == key && curr->next == nullptr) { //end
        prev->next = nullptr;
        val = curr->value;
        notfound = false;
        delete curr;
        curr = nullptr;
      } else if(prev == nullptr && curr->key == key && curr->next != nullptr){ // start
        val = curr->value;
        this->data[a%this->capacity] = curr->next;
        delete curr;
        curr = nullptr;
        notfound = false;
      } else if(prev == nullptr && curr->key == key && curr->next == nullptr) { //only one item in list
        val = curr->value;
        delete curr;
        this->data[a%this->capacity] = nullptr;
        curr = nullptr;
        notfound = false;
      }
      prev = curr;
      if (curr != nullptr) {
        curr = curr->next;
      }
    }
    if (notfound) {
      throw out_of_range("ts dont exist: erase function");
    }
    this->sz--;
    return val;
  }

  /**
   * Copy constructor.
   *
   * Copies the mappings from the provided `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings in `other`, and B is the
   * number of buckets.
   */
  HashMap(const HashMap& other) {
    // TODO_STUDENT
    this->sz = other.sz;
    this->capacity = other.capacity;
    this->data = new ChainNode*[this->capacity];
    for (int i = 0; i < this->capacity; i++) {
      ChainNode* currother = other.data[i];
      this->data[i] = nullptr;
      //ChainNode* currthis = this->data[i];
      while (currother != nullptr) {
        ChainNode* newNode = new ChainNode(currother->key, currother->value, this->data[i]);
        this->data[i] = newNode;
        //currthis = currthis->next;
        currother = currother->next;
      }
    }
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this table, and copies the mappings from the provided `HashMap`.
   *
   * Runs in O((N1+B1) + (N2+B2)), where N1 and B1 are the number of mappings
   * and buckets in `this`, and N2 and B2 are the number of mappings and buckets
   * in `other`.
   */
  HashMap& operator=(const HashMap& other) {
    // TODO_STUDENT
    if (this == &other) {
      return *this;
    }
    this->clear();
    this->sz = other.sz;
    this->capacity = other.capacity;
    delete[] this->data;
    this->data = new ChainNode*[this->capacity];
    for (int i = 0; i < this->capacity; i++) {
      ChainNode* currother = other.data[i];
      this->data[i] = nullptr;
      //ChainNode* currthis = this->data[i];
      while (currother != nullptr) {
        ChainNode* newNode = new ChainNode(currother->key, currother->value, this->data[i]);
        this->data[i] = newNode;
        //currthis = currthis->next;
        currother = currother->next;
      }
    }
    return *this;
  }

  // =====================

  /**
   * Resets internal state for an iterative traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr` and
   * `curr_idx`.
   *
   * Runs in worst-case O(B), where B is the number of buckets.
   */
  void begin() {
    // TODO_STUDENT
  }

  /**
   * Uses the internal state to return the "next" key and value
   * by reference, and advances the internal state. Returns `true` if the
   * reference parameters were set, and `false` otherwise.
   *
   * Example usage:
   *
   * ```c++
   * HashMap<string, int> hm;
   * hm.begin();
   * string key;
   * int value;
   * while (hm.next(key, val)) {
   *   cout << key << ": " << val << endl;
   * }
   * ```
   *
   * Does not visit the mappings in any defined order.
   *
   * Modifies nothing except for `curr` and `curr_idx`.
   *
   * Runs in worst-case O(B) where B is the number of buckets.
   */
  bool next(KeyT& key, ValT& value) {
    // TODO_STUDENT
    return false;
  }

  // ===============================================

  /**
   * Returns a pointer to the underlying memory managed by the `HashMap`.
   * For autograder testing purposes only.
   */
  void* get_data() const {
    return this->data;
  }

  /**
   * Returns the capacity of the underlying memory managed by the `HashMap`. For
   * autograder testing purposes only.
   */
  size_t get_capacity() {
    return this->capacity;
  }
};

#include <iostream>
#include <list>
#include <string>
using namespace std;

class HashTable {
private:
    static const int INITIAL_SIZE = 10;
    static const double LOAD_FACTOR_THRESHOLD;

    struct Entry {
        string key;
        string value;
        Entry(const string& k, const string& v) : key(k), value(v) {}
    };

    list<Entry>* table;
    int size;           // Current number of elements
    int capacity;       // Current size of the table

public:
    HashTable() {
        table = new list<Entry>[INITIAL_SIZE];
        size = 0;
        capacity = INITIAL_SIZE;
    }

    ~HashTable() {
        delete[] table;
    }

    // Hash function to compute the index for a given key
    int hashFunction(const string& key) {
        int hash = 0;
        for (char c : key) {
            hash = (hash * 31) + c; // Using prime number 31 for better distribution
        }
        return hash % capacity;
    }

    // Insert a key-value pair into the hash table
    void insert(const string& key, const string& value) {
        if (static_cast<double>(size) / capacity >= LOAD_FACTOR_THRESHOLD) {
            resize(capacity * 2);
        }

        int index = hashFunction(key);
        for (auto& entry : table[index]) {
            if (entry.key == key) {
                entry.value = value; // Update value if the key already exists
                return;
            }
        }

        table[index].emplace_back(key, value); // Add new key-value pair
        size++;
    }

    // Get the value associated with a given key
    string get(const string& key) {
        int index = hashFunction(key);
        for (auto& entry : table[index]) {
            if (entry.key == key) {
                return entry.value; // Return value if the key is found
            }
        }
        return ""; // Return an empty string if the key is not found
    }

    // Remove a key-value pair from the hash table
    void remove(const string& key) {
        int index = hashFunction(key);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->key == key) {
                table[index].erase(it);
                size--;
                if (static_cast<double>(size) / capacity <= LOAD_FACTOR_THRESHOLD / 4) {
                    resize(capacity / 2);
                }
                return;
            }
        }
    }

    // Display the contents of the hash table
    void display() {
        for (int i = 0; i < capacity; ++i) {
            cout << "Bucket " << i << ": ";
            for (const auto& entry : table[i]) {
                cout << "[Key: " << entry.key << ", Value: " << entry.value << "] ";
            }
            cout << endl;
        }
    }

private:
    // Resize the hash table to the new capacity
    void resize(int newCapacity) {
        list<Entry>* newTable = new list<Entry>[newCapacity];
        for (int i = 0; i < capacity; ++i) {
            for (const auto& entry : table[i]) {
                int newIndex = hashFunction(entry.key);
                newTable[newIndex].emplace_back(entry.key, entry.value);
            }
        }
        delete[] table;
        table = newTable;
        capacity = newCapacity;
    }
};

const double HashTable::LOAD_FACTOR_THRESHOLD = 0.75;

int main() {
    // Create a hash table
    HashTable hashTable;

    // Insert key-value pairs
    hashTable.insert("key1", "value1");
    hashTable.insert("key2", "value2");
    hashTable.insert("key3", "value3");

    // Retrieve values for keys
    cout << hashTable.get("key1") << endl; // Output: value1
    cout << hashTable.get("key2") << endl; // Output: value2
    cout << hashTable.get("key3") << endl; // Output: value3

    // Update a value
    hashTable.insert("key1", "new value");

    // Remove a key-value pair
    hashTable.remove("key2");

    // Display the contents of the hash table
    hashTable.display();

    return 0;
}

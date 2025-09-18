#ifndef CHAINHASH_H
#define CHAINHASH_H

#include <iostream>
using namespace std;

template <typename K, typename V>
class ChainHash {
private:
    struct Node {
        K key;
        V value;
        Node* next;
        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    Node** table;
    int capacity;
    int size;

    int hashFunc(const K& key) {
        unsigned long hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c;
        }
        return hash % capacity;
    }

public:
    ChainHash(int cap = 101) {
        capacity = cap;
        size = 0;
        table = new Node*[capacity];
        for (int i = 0; i < capacity; i++) table[i] = nullptr;
    }

    ~ChainHash() {
        for (int i = 0; i < capacity; i++) {
            Node* current = table[i];
            while (current) {
                Node* tmp = current;
                current = current->next;
                delete tmp;
            }
        }
        delete[] table;
    }

    void set(const K& key, const V& value) {
        int index = hashFunc(key);
        Node* current = table[index];
        while (current) {
            if (current->key == key) { 
                current->value = value;
                return;
            }
            current = current->next;
        }
        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        size++;
    }

    bool get(const K& key, V& value) {
        int index = hashFunc(key);
        Node* current = table[index];
        while (current) {
            if (current->key == key) {
                value = current->value;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool contains(const K& key) {
        int index = hashFunc(key);
        Node* current = table[index];
        while (current) {
            if (current->key == key) return true;
            current = current->next;
        }
        return false;
    }

    int bucket_count() { return capacity; }

    int bucket_size(int index) {
        int count = 0;
        Node* current = table[index];
        while (current) { count++; current = current->next; }
        return count;
    }

    // Iterador para recorrer cada bucket
    class Iterator {
    private:
        Node* node;
    public:
        Iterator(Node* n) : node(n) {}
        bool operator!=(const Iterator& other) { return node != other.node; }
        void operator++() { if (node) node = node->next; }
        Node& operator*() { return *node; }
    };

    Iterator begin(int index) { return Iterator(table[index]); }
    Iterator end(int) { return Iterator(nullptr); }
};

#endif

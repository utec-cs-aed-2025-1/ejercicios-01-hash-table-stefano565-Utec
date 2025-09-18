#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "chainhash.h"

using namespace std;

// convertir string a minusculas
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// descomponer un string en palabras
vector<string> tokenize(const string& text) {
    vector<string> words;
    stringstream ss(text);
    string word;

    while (ss >> word) {
        word.erase(remove_if(word.begin(), word.end(),
                   [](char c) { return !isalnum(c); }), word.end());
        if (!word.empty()) words.push_back(toLowerCase(word));
    }
    return words;
}

// Bag of Words usando ChainHash
ChainHash<string, vector<int>> bagOfWords(const vector<string>& documentos) {
    ChainHash<string, vector<int>> result(13);

    for (size_t docID = 0; docID < documentos.size(); docID++) {
        vector<string> words = tokenize(documentos[docID]);
        for (const string& word : words) {
            vector<int> docs;
            if (result.get(word, docs)) {
                // si ya existe, añadir documento si no está
                if (find(docs.begin(), docs.end(), docID) == docs.end())
                    docs.push_back(docID);
                result.set(word, docs);
            } else {
                // si no existe, crear nuevo vector
                docs.push_back(docID);
                result.set(word, docs);
            }
        }
    }
    return result;
}

// mostrar resultados
void printBagOfWords(ChainHash<string, vector<int>>& bow) {
    cout << "{\n";
    for (int i = 0; i < bow.bucket_count(); i++) {
        if (bow.bucket_size(i) > 0) {
            for (auto it = bow.begin(i); it != bow.end(i); ++it) {
                cout << " \"" << (*it).key << "\": [";
                const vector<int>& docs = (*it).value;
                for (size_t j = 0; j < docs.size(); j++) {
                    cout << docs[j];
                    if (j < docs.size() - 1) cout << ", ";
                }
                cout << "],\n";
            }
        }
    }
    cout << "}\n";
}

int main() {
    vector<string> documentos = {
        "La casa es grande",
        "El gato está en la casa",
        "La casa es bonita y grande",
        "El sol brilla sobre la casa"
    };

    cout << "Documentos de entrada:\n";
    for (size_t i = 0; i < documentos.size(); i++) {
        cout << "[" << i << "] \"" << documentos[i] << "\"\n";
    }
    cout << "\n";

    ChainHash<string, vector<int>> bow = bagOfWords(documentos);
    cout << "Resultado de Bag of Words (palabra -> [documentos]):\n";
    printBagOfWords(bow);

    return 0;
}

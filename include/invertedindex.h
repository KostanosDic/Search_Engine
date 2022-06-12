#pragma once
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "lowercasemaker.h"
#include <algorithm>

struct Entry
{
    size_t doc_id;
    size_t count;

    bool operator ==(const Entry& other) const
    { return (doc_id == other.doc_id && count == other.count); }
};

class InvertedIndex
{
 public:
    InvertedIndex() = default;
    void UpdateDocumentBase(std::vector<std::string> inputDocs);
    std::vector<Entry> GetWordCount(const std::string& word);
    friend std::vector<std::string> getDocs( const InvertedIndex& IndexInverted);

 private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freqDictionary;
};

std::vector<std::string> getDocs( const InvertedIndex& );


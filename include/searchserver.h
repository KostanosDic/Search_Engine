#pragma once
#include "invertedindex.h"
#include "converterjson.h"

struct RelativeIndex
{
    size_t doc_id;
    float rank;

    bool operator ==(const RelativeIndex& other) const
    { return (doc_id == other.doc_id && rank == other.rank); }
};

std::vector<std::vector<std::pair<int, float>>> structToPairConvert(const std::vector<std::vector<RelativeIndex>>& indexes);

class SearchServer
{
 public:
    SearchServer(InvertedIndex& idx) : _index(idx){};
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queriesInput);

 private:
    InvertedIndex _index;
};


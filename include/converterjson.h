
#pragma once
#include <string>
#include <vector>
#include <conio.h>
#include <stdio.h>

class ConverterJSON
{
 public:
    ConverterJSON() = default;

    std::vector<std::string> GetTextDocuments();

    int GetResponsesLimit();

    std::vector<std::string> GetRequests();

    void putAnswers( std::vector< std::vector< std::pair<int, float> > > answers );
};


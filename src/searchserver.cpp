#include "searchserver.h"
#include <cmath>
#include <iostream>

std::vector<std::vector<std::pair<int, float>>> structToPairConvert(const std::vector<std::vector<RelativeIndex>>& indexes)
{
    std::vector<std::vector<std::pair<int, float>>> result;

    for(auto& vec: indexes)
    {
        std::vector<std::pair<int, float>> tmp;
        tmp.reserve(vec.size());
        for(auto& idx : vec)
        {
            tmp.emplace_back(idx.doc_id, idx.rank);
        }
        result.push_back(tmp);
    }
    return result;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queriesInput)
{
    std::vector<std::vector<RelativeIndex>> requestRelevanceVectors;
    std::vector<std::string> requiresInputVec;

    int requestExceedWordNumber = 0;
    for( int t = 0; t < queriesInput.size(); ++t)
    {
        requiresInputVec.erase( requiresInputVec.begin() , requiresInputVec.end() );
        std::istringstream currentRequestStream( queriesInput[t] );

        int requestWordNumber = 0;
        while( !currentRequestStream.eof() )
        {
            std::string wordCurrent;
            currentRequestStream >> wordCurrent;
            requestWordNumber++;
            for_each( wordCurrent.begin(), wordCurrent.end() , LowerCaseMaker() );
            if( wordCurrent != "" ) requiresInputVec.push_back(wordCurrent);
        }
        if( requestWordNumber > 10 ) requestExceedWordNumber++;

        std::sort( requiresInputVec.begin(), requiresInputVec.end() );
        requiresInputVec.erase( std::unique( requiresInputVec.begin(), requiresInputVec.end() ), requiresInputVec.end() );

        std::vector <RelativeIndex> currentReqRelevanceVec;
        std::vector <float> docsCounterVec ( getDocs(_index).size() );

        if( requiresInputVec.size() )
        {
            for( int i = 0; i < requiresInputVec.size(); ++i )
            {
                for( int k = 0; k < ( _index.GetWordCount(requiresInputVec[i]) ).size(); ++k )
                {
                    docsCounterVec[ _index.GetWordCount(requiresInputVec[i])[k].doc_id ] += _index.GetWordCount(requiresInputVec[i])[k].count;
                }
            }
        }

        float docsCounterVecMaxElem = 0;
        for(int i = 0; i < docsCounterVec.size(); ++i)
        {
            if( docsCounterVec[i] > 0 )
            {
                RelativeIndex currentRelativeIndex;
                currentRelativeIndex.doc_id = i;
                currentRelativeIndex.rank = docsCounterVec[i];
                if( currentRelativeIndex.rank > docsCounterVecMaxElem ) docsCounterVecMaxElem = currentRelativeIndex.rank;
                currentReqRelevanceVec.push_back( currentRelativeIndex );
            }
        }

        if( docsCounterVecMaxElem != 0 )
        {
            for( int i = 0; i < currentReqRelevanceVec.size(); ++i) currentReqRelevanceVec[i].rank /= docsCounterVecMaxElem;

            std::sort( currentReqRelevanceVec.begin(), currentReqRelevanceVec.end(), [](RelativeIndex elem1, RelativeIndex elem2) {if( fabs(elem1.rank - elem2.rank) > 1.0E-06 ){return elem1.rank > elem2.rank;} else {return elem1.doc_id < elem2.doc_id;}  } );
            ConverterJSON getRequestsTempClass;
            if( currentReqRelevanceVec.size() > getRequestsTempClass.GetResponsesLimit() ) currentReqRelevanceVec.erase( currentReqRelevanceVec.begin() + getRequestsTempClass.GetResponsesLimit(), currentReqRelevanceVec.end() );
        }
        else
        {
            currentReqRelevanceVec.erase( currentReqRelevanceVec.begin(), currentReqRelevanceVec.end() );
        }
        requestRelevanceVectors.push_back( currentReqRelevanceVec );
    }

    if(requestExceedWordNumber)
    {
        std::cout << "\n\nWARNING! " << requestExceedWordNumber  << " requests contains more than 10 words. Possibility of memory overflow!!!\n\n" << std::endl;
    }

    return requestRelevanceVectors;
}

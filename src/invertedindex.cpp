#include "invertedindex.h"
#include <thread>
#include <mutex>
#include <future>
#include <iostream>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word)
{
    std::vector<Entry> tempEntryVector;
    if( freqDictionary.find(word) != freqDictionary.end() ) return freqDictionary.find(word)->second;
    else return  tempEntryVector;
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> inputDocs)
{
    std::vector <std::thread> threads;
    std::mutex indexWriteGuard;

    docs = inputDocs;
    int docsExceedWordNumber = 0;
    int wordExceedLettersNumber = 0;
    boost::asio::thread_pool pool(std::thread::hardware_concurrency());

    for(int i = 0; i < inputDocs.size(); ++i) {
        boost::asio::post(pool,[this, &inputDocs, &indexWriteGuard, &docsExceedWordNumber, &wordExceedLettersNumber, i]() {

                              std::vector<std::string> currentDocWords;
                              currentDocWords.erase(currentDocWords.begin(), currentDocWords.end());
                              std::istringstream currentDocsStream(inputDocs[i]);

                              int inputDocsWordNumber = 0;
                              while (!currentDocsStream.eof()) {
                                  std::string wordCurr;
                                  currentDocsStream >> wordCurr;
                                  inputDocsWordNumber++;
                                  if (wordCurr.size() > 100) wordExceedLettersNumber++;
                                  for_each(wordCurr.begin(), wordCurr.end(), LowerCaseMaker());
                                  if (wordCurr != "") currentDocWords.push_back(wordCurr);
                              }
                              if (inputDocsWordNumber > 1000) docsExceedWordNumber++;

                              std::vector<std::string> temp_zip_str;
                              temp_zip_str = currentDocWords;
                              std::sort(temp_zip_str.begin(), temp_zip_str.end());
                              temp_zip_str.erase(std::unique(temp_zip_str.begin(), temp_zip_str.end()),
                                                 temp_zip_str.end());

                              Entry tempEntry;
                              for (int j = 0; j < temp_zip_str.size(); ++j) {
                                  int currNumberWord = std::count(currentDocWords.begin(), currentDocWords.end(),
                                                                  temp_zip_str[j]);
                                  tempEntry.count = currNumberWord;
                                  tempEntry.doc_id = i;

                                  indexWriteGuard.lock();
                                  if (freqDictionary.find(temp_zip_str[j]) == freqDictionary.end()) {
                                      std::vector<Entry> tempVector;
                                      tempVector.push_back(tempEntry);
                                      freqDictionary.insert(
                                              std::pair<std::string, std::vector<Entry> >(temp_zip_str[j], tempVector));
                                  } else {
                                      auto itr = freqDictionary.find(temp_zip_str[j]);
                                      itr->second.push_back(tempEntry);
                                  }
                                  indexWriteGuard.unlock();

                              }
                          });

    }
    pool.join();

    if(docsExceedWordNumber)
    {
        std::cout << "\n\nWARNING! " << docsExceedWordNumber  << " files contains more than 1000 words. Possibility of memory overflow!!!\n\n" << std::endl;
    }
    if(wordExceedLettersNumber)
    {
        std::cout << "\n\nWARNING! " << wordExceedLettersNumber  << " words in documents contains more than 100 letters. Possibility of memory overflow!!!\n\n" << std::endl;
    }

    std::vector< std::future<void> > futures;
    for(std::map< std::string, std::vector<Entry> >::iterator i = freqDictionary.begin(); i != freqDictionary.end(); ++i)
    {
        auto currentFuture = std::async( std::launch::async, [i]() {
            std::sort( (i->second).begin(), (i->second).end(), [](Entry elem1, Entry elem2) {return elem1.doc_id < elem2.doc_id; } );
        });

        futures.emplace_back(std::move(currentFuture));
    }
    for(auto& curFuture : futures) if( curFuture.valid() ) curFuture.get();

}

std::vector<std::string> getDocs( const InvertedIndex& IndexInverted)
{
    return IndexInverted.docs;
}

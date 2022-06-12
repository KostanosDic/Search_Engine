#include "converterjson.h"
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include <exceptions.h>
#include <io.h>
#include <sstream>
#include <iomanip>

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
    std::ifstream configFile;
    nlohmann::json jsonConfig;
    ////////////////////////////////////////////////////
    try
    {
        if( access( "../config.json" , 0 ) == -1 ) throw FileMissingException();
        configFile.open("../config.json");
        if( ! configFile ) throw FileOpenException();
    }
    catch (const FileOpenException& openFileError)
    {
        std::cerr << "config " << openFileError.what() << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }
    catch (const FileMissingException& missingFileError)
    {
        std::cerr << "config " << missingFileError.what() << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }
    //////////////////////////////////////////////////////
    try
    {
        jsonConfig = nlohmann::json::parse(configFile);
    }
    catch (nlohmann::json::parse_error& parseError)
    {
        std::cerr << "config-file parse error: " << parseError.what() << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }
    //////////////////////////////////////////////////////////////////////////////////
    try
    {
        if( jsonConfig["config"] == nullptr ) throw NoFieldException();
    }
    catch (const NoFieldException& lostConfigField )
    {
        std::cerr << "config " << lostConfigField.what() << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }
    //////////////////////////////////////////////////////////////////////////////////
    if(jsonConfig["config"] ["name"] == nullptr || !(jsonConfig["config"] ["name"].is_string()) )
    {
        std::cout <<"Starting" <<  std::endl << std::endl;
    }
    else
    {
        std::cout <<"Starting  " << (std::string)jsonConfig["config"] ["name"] <<  std::endl << std::endl;
    }

    ////////////////////////////////////////
    if( jsonConfig["files"] == nullptr || !jsonConfig["files"].is_array() )
    {
        std::cerr <<" No json-field FILES is included inside config file! We are completing...." << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }
    if( jsonConfig["files"].size() == 0 )
    {
        std::cerr <<" No files file00n.txt are included inside config file! We are completing...." << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }

    std::vector <std::string> docFilesContent;
    for(int i = 0; i < jsonConfig["files"].size(); ++i)
    {
        std::string wholeFileText = "";
        if( !( jsonConfig["files"][i].is_string() ) )
        {
            std::cerr << std::endl << "File " << jsonConfig["files"][i] << " can not be open. Using empty string..." << std::endl;
            docFilesContent.push_back(wholeFileText);
            continue;
        }
        char buff[512];
        std::ifstream filesToSearch;
        filesToSearch.open((std::string)(jsonConfig["files"][i]) );

        if( ! filesToSearch )
        {
            std::cerr << std::endl << "File " << (std::string)(jsonConfig["files"][i]) << " can not be open ..." << std::endl;
            docFilesContent.push_back(wholeFileText);
        }
        else
        {
            do
            {
                filesToSearch.read(buff, 512);
                wholeFileText.append(buff, filesToSearch.gcount());

            } while( !filesToSearch.eof() );

            filesToSearch.close();
            docFilesContent.push_back(wholeFileText);
        }

    }

    configFile.close();
    return docFilesContent;
}

int ConverterJSON::GetResponsesLimit()
{
    std::ifstream configFile;
    nlohmann::json jsonConfig;
    ////////////////////////////////////////////////////
    try
    {
        if( access( "../config.json" , 0 ) == -1 ) throw FileMissingException();
        configFile.open("../config.json");
        if( ! configFile ) throw FileOpenException();
    }
    catch (const FileOpenException& openFileError)
    {
        std::cerr << "config " << openFileError.what() << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }
    catch (const FileMissingException& missingFileError)
    {
        std::cerr << "config " << missingFileError.what() << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }
    //////////////////////////////////////////////////////
    try
    {
        jsonConfig = nlohmann::json::parse(configFile);
    }
    catch (nlohmann::json::parse_error& parseError)
    {
        std::cerr << "config-file parse error: " << parseError.what() << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }
    //////////////////////////////////////////////////////////////////////////////////
    try
    {
        if( jsonConfig["config"] == nullptr ) throw NoFieldException();
    }
    catch (const NoFieldException& lostConfigField )
    {
        std::cerr << "config " << lostConfigField.what() << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }

    ////////////////////////////////////////
    if( jsonConfig["config"]["max_responses"] == nullptr || !(jsonConfig["config"]["max_responses"].is_number_integer() ) || jsonConfig["config"]["max_responses"].empty() || jsonConfig["config"]["max_responses"].is_null() )
    {
        return 5;
    }
    else
    {
        return jsonConfig["config"]["max_responses"];
    }

}

std::vector<std::string> ConverterJSON::GetRequests()
{
    std::ifstream requestsFile;
    nlohmann::json jsonRequests;
    ////////////////////////////////////////////////////
    try
    {
        if( access( "../requests.json" , 0 ) == -1 ) throw FileMissingException();
        requestsFile.open("../requests.json");
        if( ! requestsFile ) throw FileOpenException();
    }
    catch (const FileOpenException& openFileError)
    {
        std::cerr << "requests " << openFileError.what() << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }
    catch (const FileMissingException& missingFileError)
    {
        std::cerr << "requests " << missingFileError.what() << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }
    //////////////////////////////////////////////////////
    try
    {
        jsonRequests = nlohmann::json::parse(requestsFile);
    }
    catch (nlohmann::json::parse_error& parseError)
    {
        std::cerr << "requests-file parse error: " << parseError.what() << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }
    //////////////////////////////////////////////////////////////////////////////////
    try
    {
        if( jsonRequests["requests"] == nullptr || jsonRequests["requests"].empty() || !jsonRequests["requests"].is_array() ) throw NoFieldException();
    }
    catch (const NoFieldException& lostRequestsField )
    {
        std::cerr << "requests " << lostRequestsField.what() << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }

    std::vector <std::string> requestFilesContent;

    if( jsonRequests["requests"].size() > 1000 )
    {
        std::cout << "\n\nWARNING! File requests.json contains more than 1000 requests. Possibility of memory overflow!!!\n\n" << std::endl;
    }
    for(int j = 0; j < jsonRequests["requests"].size(); ++j)
    {
        std::string wholeRequestText = "";
        if( !( jsonRequests["requests"][j].is_string() ) )
        {
            std::cerr << std::endl << "Request " << jsonRequests["requests"][j] << " is not a string. Using empty string instead..." << std::endl;
            requestFilesContent.push_back(wholeRequestText);
            continue;
        }

        wholeRequestText = (std::string)jsonRequests["requests"][j];
        requestFilesContent.push_back(wholeRequestText);
    }

    return requestFilesContent;
}

void ConverterJSON::putAnswers( std::vector< std::vector< std::pair<int, float> > > answers )
{
    nlohmann::ordered_json answresJsonObject;
    //if( access( "../answers.json" , 0 ) != -1 )
    if( !access( "../answers.json" , 0 ) )
    {
        if( remove( "../answers.json") != 0 )
        {
            std::cout << "\n\nError updating file answers.json, Please provide access to it and start again !!!\n\n" << std::endl;
            std::cout << "\nPress any key ... " << std::endl;
            getch();
            exit(EXIT_FAILURE);
        }
    }
    std::ofstream answresJsonFile;
    try
    {
        answresJsonFile.open("../answers.json");
        if( ! answresJsonFile ) throw FileOpenException();
    }
    catch (const FileOpenException& openFileError)
    {
        std::cerr << "answers " << openFileError.what() << std::endl;
        std::cout << "\nPress any key ... " << std::endl;
        getch();
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < answers.size(); ++i)
    {
        std::ostringstream requestIdField;
        std::string requestIdFieldString = "request";
        if( i+1 < 1000)  requestIdField << std::setw(3) << std::setfill('0') << i+1;
        if( i+1 >= 1000) requestIdField << std::setw(4) << std::setfill('0') << i+1;
        requestIdFieldString += requestIdField.str();

        if( !answers[i].size() )
        {
            answresJsonObject["answers"][requestIdFieldString]["result"] = "false";
        }
        else if(answers[i].size() == 1)
        {
            answresJsonObject["answers"][requestIdFieldString]["result"] = "true";
            answresJsonObject["answers"][requestIdFieldString]["docid"] = answers[i][0].first;
            std::stringstream docrank;
            docrank << std::setiosflags(std::ios::showpoint | std::ios::left) << std::setprecision(3) << std::setw(5) << answers[i][0].second;
            answresJsonObject["answers"][requestIdFieldString]["rank"] = round((double)( std::stod( docrank.str() ) ) * 1000) / 1000;
        }
        else
        {
            answresJsonObject["answers"][requestIdFieldString]["result"] = "true";
            for (int j = 0; j < answers[i].size(); ++j)
            {
                nlohmann::ordered_json JsonObject;
                JsonObject["docid"] = answers[i][j].first;
                std::stringstream docrank;
                docrank << std::setiosflags(std::ios::showpoint | std::ios::left) << std::setprecision(3) << std::setw(5) << answers[i][j].second;
                JsonObject["rank"] = round((double)( std::stod( docrank.str() ) ) * 1000) / 1000;
                answresJsonObject["answers"][requestIdFieldString]["relevance"].push_back(JsonObject);
            }
        }
    }
    answresJsonFile << answresJsonObject.dump(7);
    std::cout << "\n\nFile answers.json is writen !!!" << std::endl;

}

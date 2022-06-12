# SearchEngine
Search Engine for finding the words inside text files. Gives out the files, which contains all/any of words from the user requests, and calculates relative frequency of user-request words appearance inside text files. Input and output are realized using JSON format.
# Platforms
 + Windows 10 (64-bit) or later
 + Ubuntu Linux 20.04 (64-bit) or later
# Prerequisites
 + On Windows: MinGW X64 with GCC 8.1.0 or later  
 + On Linux: GCC 8.1 or later  
 + Cmake 3.21 or later  
 + JSON for Modern C++ version 3.10.5 or later
 + Boost C++ Libraries 1.79.0 or later 
 + Google Test (GTest)  
 + Ninja build system 1.10.2 or later  
 # Prepare for build
 + Load current repository on Your hard drive and unzip. Enter the folder containing this repository - it will be referenced here as "working folder" (the default working folder name - **Search_Engine-main** - may be changed if required).
 + Install JSON for Modern C++ ( Copyright (c) 2013-2022 Niels Lohmann ) ( Licensed under the MIT License https://json.nlohmann.me/home/license/ ) to the working folder, and rename the library folder to "nlohmann_json" (it is mandatory).
 + Install Boost C++ Libraries ( Distributed under the Boost Software License, Version 1.0. http://www.boost.org/LICENSE_1_0.txt ) to the working folder, and edit **CMakeLists.txt** file to make it keep correct name of the Boost C++ Libraries folder (for example: **"boost_1_79_0"**, according to installed Version number), as shown on the illustration below. Also make sure that ws2_32-library is included to both **"target_link_libraries"** commands, if required (if Windows + MinGW are used).  
<p align="center">
  <img src="https://github.com/KostanosDic/Search_Engine/blob/main/pic/0.png?raw=true" width="750" title="Boost library version writing to the paths CMakeLists.txt">
</p>

 + Add the follow to Your user/system environment variable "Path":
   + <full path to the **cmake.exe** file, but excluding the executable itself (at CMake installation location) >
   + <full path to the **ninja.exe** file, but excluding the executable itself (at Ninja installation location) >
   + <full path to the folder, containing files **g++.exe**, **gcc.exe** (at MinGW/gcc installation location )>
 # Build
 ```
 chdir [/D] <full path to Your "working folder">  
 cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -S . -B "build64" -DCMAKE_MAKE_PROGRAM=ninja -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++  
 cmake --build build64 --config Release  
 ```
 See the example of build this Search Engine project using Windows x64 OS on the illustration below  
<p align="center">
  <img src="https://github.com/KostanosDic/Search_Engine/blob/main/pic/1.png?raw=true" width="1200" title="Windows x64 build example">
</p>

The new folder named "**build64**" appears inside the working folder. It contains the SearchEngine executable file (**"..\build64\SearchEngine.exe"** ). If the command window/shell screen has such a view as on the illustration below after runnung this file, everithing is OK - build is successful:  

<p align="center">
  <img src="https://github.com/KostanosDic/Search_Engine/blob/main/pic/2.png?raw=true" width="270" title="command window after SearchEngine executable run">
</p>

 # How to start search
 The Search Engine information input is performed using JSON-format files. The typical view of **config.json** file is shown on the illustration below:  
 
 <p align="center">
  <img src="https://github.com/KostanosDic/Search_Engine/blob/main/pic/3.png?raw=true" width="450" title="the typical view of config.json file">
</p>

The JSON-field named "config" contains folowing fileds:  
 + "name" - the name of program (may be changed by user)  
 + "version" - version of the program (may be changed by user)  
 + "nextupdate" - date for next inverted index base updating (currently not used)  
 + "max_responses" - maximum number of text docs, satisfying to every user's request, which can be printed to the output file (equals to 5 by default)  

The JSON-field named "files" contains JSON-array with paths to the files. That files contain the text docs for perform the search among it contents. As we can see, every file should be placed to the "resources" folder inside the working folder. Every text field of "files"  JSON-array should have the form: "**../resources/<file_name>.txt**".  

**NOTE, THAT SEQUENCE ORDER OF FILE PATHS IS IMPORTANT FOR RESULTS REPRESENTATION.** The integer identifier "docid" is assigned to every file, and definitely identifies the text file at search results view.

Note, that every text document should not contain more than 1000 words, separated by spaces, every word length should not exceed 100 latin letters. Otherwise, warnings will be generated. 

The typical view of **requests.json** file is shown on the illustration below:  
 
 <p align="center">
  <img src="https://github.com/KostanosDic/Search_Engine/blob/main/pic/4.png?raw=true" width="400" title="the typical view of requests.json file">
</p>

The JSON-field named "requests" contains JSON-array with text strings. Every such a string is the user's request, i.e. collection of latin letter words to search their appearance among content of all text files, pointed above (see **config.json** above). The JSON-field "requests" should not contain more than 1000 requests, every request should not contain more than 10 latin letters words, separated by spaces. Otherwise, warnings will be generated.  

**NOTE, THAT SEQUENCE ORDER OF USER'S REQUESTS STRINGS IS IMPORTANT FOR RESULTS REPRESENTATION.** The string identifier "requestNNN" (where NNN = 001, 002, ... ,1000), completely identifying the user's request at searsh results view, is assigned to every request.

The scheme at the illustration below explains the way the search results JSON-file (**answers.json**) is formed:  

 <p align="center">
  <img src="https://github.com/KostanosDic/Search_Engine/blob/main/pic/5.png?raw=true" width="1200" title="the scheme of search results file answers.json ">
</p>

As it can be fnid from illustration, the **answers.json** file contains the JSON-fields "request001", "request002" and so on, and serial number of every such a field definitely corresponds to serial number of some user's request (from **requests.json** file). The serial numbers are counted from 1.  
Inside every JSON-field "requestNNN" (where NNN = 001, 002, ... ,1000) the "docid"-identifiers along with the "rank"-values are contained (if field **"result" = "true"**). The "rank" value is the "relevance" value of document with current "docid"-identifier for current request "requestNNN". In fact, "rank"-value is the relative frequency of words from current "requestNNN" appearance among the content of text document with current "docid"-identifier (following the order of fields inside **config.json** and **requests.json** files ).  

**The docid"-identifiers with the "rank" values appear inside JSON-field "requestNNN" (where NNN = 001, 002, ... ,1000) in descending order of "rank" value magnitude.**  

If field **"result" = "false"** inside JSON-field "requestNNN" (where NNN = 001, 002, ... ,1000) appears, it means that no words from request "requestNNN" were found in text files inside "resources" folder.  

After editing **config.json** and **requests.json** files, and also text files at **"resources"** folder, run of **"..\build64\SearchEngine.exe"** should invoke **answers.json** file of above-mentioned form to appear inside the working folder.

# Tests
 + Included files **config.json** and **requests.json** contain test named "TestSimple" by default, so running **"..\build64\SearchEngine.exe"** after build will invoke **answers.json** appearance in the working folder with that test results.  
 + Run **"..\build64\SearchEngineTest.exe"** in Your command window/shell to try all five Tests provided.


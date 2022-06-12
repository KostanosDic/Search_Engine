#include "searchserver.h"
#include <iostream>

int main()
{
    ConverterJSON ConverterJsonObject;
    InvertedIndex InvertedIndexObject;
    InvertedIndexObject.UpdateDocumentBase(ConverterJsonObject.GetTextDocuments());
    SearchServer SearchServerObject(InvertedIndexObject);
    ConverterJsonObject.putAnswers( structToPairConvert(SearchServerObject.search(ConverterJsonObject.GetRequests())) );
    std::cout << std::endl << "Press any key to exit ... " << std::endl;

    getch();
    return 0;

}

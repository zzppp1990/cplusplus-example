/*
 saveGeneralizeTp = new SaveGeneralizeTp();
auto generalizeData = saveGeneralizeTp->CurXoscUseTemplate(xoscFilename);
generalizeObjectVec.emplace_back(generalizeData);
*/

#include "SaveGeneralizeTp.h"
#include <iostream>
using namespace std;

void printGeneralizeTag(vector<GeneralizeTag>& vecGeneralizeTag)
{
    for(size_t i = 0; i < vecGeneralizeTag.size(); ++i)
    {
        cout << "\t" << vecGeneralizeTag[i].tagName << "," << vecGeneralizeTag[i].beginValue << "," << vecGeneralizeTag[i].endValue << endl;
    }
}

void printMap(map<string, vector<GeneralizeTag>>& pmap)
{
    map<string, vector<GeneralizeTag>>::iterator it = pmap.begin();
    for(; it != pmap.end(); ++it)
    {
        cout << "\t" << it->first << endl;
        printGeneralizeTag(it->second);
    }
}

int main()
{
    SaveGeneralizeTp* saveGeneralizeTp = new SaveGeneralizeTp();
    //string xoscFilename = "/home/saimo/AosDemoTest1/zp/fanhua_test/xmlparse/yanshi.xosc";

    string xoscFilename = "/home/saimo/AosDemoTest1/zp/fanhua_test/xmlparse/shijian.xosc";

#if 0
    GeneralizeData<GeneralizeTag> generalizeData = saveGeneralizeTp->CurXoscUseTemplate(xoscFilename);
    /*print map*/
    map<string, map<string, vector<GeneralizeTag>>>::iterator it = generalizeData.vehicleEventMap.begin();

    for(; it != generalizeData.vehicleEventMap.end(); ++it)
    {
        cout << "---------" << endl;
        cout << it->first << endl;
        printMap(it->second);
    }
#endif

#if 0
    saveGeneralizeTp->testGetBaseTag(xoscFilename);
#endif

#if 1
    GeneralizeData<GeneralizeTag> generalizeData = saveGeneralizeTp->CurXoscUseTemplate(xoscFilename);
    printMap(generalizeData.egoEventTagMap);
#endif
    return 0;
}
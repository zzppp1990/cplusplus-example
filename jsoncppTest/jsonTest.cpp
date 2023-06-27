#include <string>
#include <jsoncpp/json/json.h>
#include <iostream>
 
using namespace std;

void getPodNameList(string& resultlist, vector<string>& podnamelist)
{
    Json::Reader reader;
    Json::Value value;
    if (reader.parse(resultlist, value))
    {
        std::string t_podname = "";
        Json::Value arrayObj = value;

        for (unsigned int i = 0; i < arrayObj.size(); i++)
        {
            if (!arrayObj[i].isMember("podname")) 
                continue;
            t_podname = arrayObj[i]["podname"].asString();
            if(string::npos == t_podname.find("deleted"))
            {
                podnamelist.push_back(t_podname);
                arrayObj[i]["podname"] = t_podname+"deleted";
            }
        }
        resultlist = arrayObj.toStyledString();
	}
    else
    {
        std::cout << "resultlist parse failed" << std::endl;
    }
}


void readJson() {
    //std::string strValue = "{\"uuid\":\"123456\",\"resultlist\":[{\"scenid\":\"1\"},{\"evaret\":\"2\"},{\"podname\":\"pod_1\"}]}";
    std::string strValue = "[{\"scenid\":\"1\"},{\"evaret\":\"2\"},{\"podname\":\"pod_1\"},{\"scenid\":\"2\"},{\"evaret\":\"3\"},{\"podname\":\"pod_2\"}]";
    //std::string strValue = "[]";
    vector<string> podnamelist;
    getPodNameList(strValue, podnamelist);
    for(int i = 0; i < podnamelist.size(); ++i)
    {
        std::cout << "pod name to del : " << podnamelist[i] << std::endl;
    }
    std::cout << strValue << std::endl;
}
 
void writeJson() {
 
	Json::Value root;
	Json::Value arrayObj;
	Json::Value item;
 
	item["cpp"] = "jsoncpp";
	item["java"] = "jsoninjava";
	item["php"] = "support";
	arrayObj.append(item);
 
	root["name"] = "json";
	root["array"] = arrayObj;
 
	root.toStyledString();
	std::string out = root.toStyledString();
	std::cout << out << std::endl;
}
 
int main(int argc, char** argv) {
	readJson();
	//writeJson();
	return 0;
}
#ifndef SAVEGENERALIZETP_H
#define SAVEGENERALIZETP_H

#include <string.h>
#include <list>
#include <map>
#include <vector>
#include "simproxml.h"
#include <string>
using std::list;
using std::map;
using std::string;
using std::vector;

/// @brief 一个标签的起始值，结束值，步长，标签名的结构体
struct GeneralizeTag
{
    /// @brief 标签名称
    string tagName;
    /// @brief 起始值
    double beginValue;
    /// @brief 结束值
    double endValue;
    /// @brief 步长
    double stepValue;
};

/// @brief 生成泛化文件样例
struct GeneralizeSample
{
    string tagName;
    double val;
};

/// @brief 一个场景文件里的所有可泛化标签和标签的起始值，结束值，步长，标签名的结构体
template <typename T>
struct GeneralizeData
{
    /// @brief 场景文件名称
    string xoscFileName;
    /// @brief 地图文件名称
    string logicFileName;
    /// @brief 可视化数据库文件名称
    string sceneGraphFileName;
    /// @brief 场景文件中环境车和行人障碍物列表
    // vector<string> objectList;
    /// @brief 主车基本参数列表 tag
    vector<T> egoBaseTagList;
    /// @brief 主车关联事件Map event tag
    map<string, vector<T>> egoEventTagMap;
    /// @brief 环境车基本参数列表 veh_name tag
    map<string, vector<T>> vehicleTagMap;
    /// @brief 环境车事件列表 event veh_name tag
    map<string, map<string, vector<T>>> vehicleEventMap;
    /// @brief 行人基本参数列表 pedestrian_name tag
    map<string, vector<T>> pedestrianTagMap;
    /// @brief 行人事件列表 event pedestrian_name tag
    map<string, map<string, vector<T>>> pedestrianEventMap;
    /// @brief 障碍物基本参数列表 misc_name tag
    map<string, vector<T>> miscobjectTagMap;
    /// @brief 环境参数列表
    vector<T> envObjectTagVector;
};
template <typename T>
bool isNull(GeneralizeData<T> &generalizeData)
{
    
    if (generalizeData.egoBaseTagList.empty() && generalizeData.egoEventTagMap.empty() && generalizeData.vehicleTagMap.empty()
     && generalizeData.vehicleEventMap.empty() && generalizeData.pedestrianTagMap.empty() && generalizeData.pedestrianEventMap.empty()
     && generalizeData.miscobjectTagMap.empty() && generalizeData.envObjectTagVector.empty())
     {
        return true;
     }

     return false;

}


class SaveGeneralizeTp
{
private:
    map<string, vector<string>> _baseNote;
    map<string, vector<string>> _relativeNote;
    map<string, vector<double>> _range;
    string filepath;


public:
    SaveGeneralizeTp(/* args */);
    ~SaveGeneralizeTp() = default;
    bool IsTemplateExist(string _file);
    int SaveDataToXml(const GeneralizeData<GeneralizeTag> &tpd);
    GeneralizeData<GeneralizeTag> GetDataFromXml(const string &file);
    /// @brief 返回选择文件的标签内容
    /// @param xoscfile 文件名
    /// @return 可泛化标签与默认范围
    GeneralizeData<GeneralizeTag> CurXoscUseTemplate(const string &xoscfile);
    /// @brief 获取列表标签
    vector<GeneralizeTag> GetBaseTag(simproxml::XMLElement *element, bool &isEventExist, bool isBaseNote = true, char **eventName = nullptr);
    /// @brief 设置保存文件路径
    /// @param path
    void SetFilePath(string path);
    
    /// @brief 返回场景文件中包含的实体，区分车辆、行人、障碍物
    /// @param entities标签
    map<string, string> getEntities(simproxml::XMLElement *entitiesEle);


    // test
    void testGetBaseTag(const string &xoscfile);
    vector<GeneralizeTag> GetBaseTagTmp(simproxml::XMLElement *element, bool &isEventExist, bool isBaseNote = true, char **eventName = nullptr);

};

#endif
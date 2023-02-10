#include "SaveGeneralizeTp.h"
#include <vector>
#include <tuple>
#include <unistd.h>
#include <set>
#include <algorithm>
#include <stack>
//#include "Common.h"

using std::set;
using std::stack;
using std::tuple;
using std::vector;

SaveGeneralizeTp::SaveGeneralizeTp(/* args */)
{
    _range = {
        //基本参数
        std::make_pair("basespeed", vector<double>{60, 80, 20}), // value   初速度
        std::make_pair("bases", vector<double>{0, 20, 1}),   // s       位置（s)
        std::make_pair("baset", vector<double>{0, 2, 0.1}),      // offset  位置（t)
        std::make_pair("basexs", vector<double>{0, 100, 20}),  // ds      相对距离
        std::make_pair("basetp", vector<double>{-1, 1, 0.25}),     // offset  偏置率
        //关联事件
        std::make_pair("eventjhbj", vector<double>{5, 100, 10}), // value   激活半径
        std::make_pair("eventpzsj", vector<double>{3, 10, 1}),   // value   碰撞时间
        std::make_pair("eventctsj", vector<double>{1, 3, 0.1}),   // value   车头时距
        std::make_pair("eventjhsd", vector<double>{10, 50, 10}),   // value   激活速度
        std::make_pair("eventxsjl", vector<double>{100, 500, 100}),  // value   行驶距离
        std::make_pair("eventfzsc", vector<double>{10, 100, 10}),  // value   仿真时长
        std::make_pair("eventys", vector<double>{0, 10, 1}),    // delay   延时
        std::make_pair("eventbdfw", vector<double>{-2, 2, 1}),  // value   变道方位
        std::make_pair("eventbdsj", vector<double>{3, 10, 1}),  // value   变道时间
        std::make_pair("eventzxxpy", vector<double>{-3, 3, 1}), // targetLaneOffset    中心线偏移
        std::make_pair("eventjsd", vector<double>{1, 8, 1}),      // value   加速度
        std::make_pair("eventmbsd", vector<double>{30, 80, 20}),   // value   目标速度
        std::make_pair("eventcdpyl", vector<double>{-3, 3, 1}), // value   车道偏移量
        std::make_pair("eventzdhxjsd", vector<double>{1, 8, 1}),  // maxLateralAcc   最大横向加速度
        std::make_pair("Time", vector<double>{10, 22, 1}),        // dateTime    时间
        std::make_pair("Rain", vector<double>{1, 100, 10}),        // intensity   雨 雪
        std::make_pair("Snow", vector<double>{1, 100, 10}),
        std::make_pair("Fog", vector<double>{0, 100, 10}),       // visualRange 雾
        std::make_pair("Friction", vector<double>{0.01, 1, 0.1}), // frictionScaleFactor 摩擦系数
    };

    _baseNote = {
        ///基本参数
        {"AbsoluteTargetSpeed", {"basespeed"}},
        {"LanePosition", {"bases", "baset"}},
        {"RelativeLanePosition", {"basexs", "basetp"}},
    };

    _relativeNote = {
        ///关联事件
        {"DistanceCondition", {"eventjhbj"}},
        {"TimeToCollisionCondition", {"eventpzsj"}},
        {"TimeHeadwayCondition", {"eventctsj"}},
        {"SpeedCondition", {"eventjhsd"}},
        {"TraveledDistanceCondition", {"eventxsjl"}},
        {"SimulationTimeCondition", {"eventfzsc"}},
        //{"delay", {"eventys"}},
        {"Condition", {"eventys"}},
        {"RelativeTargetLane", {"eventbdfw"}},
        {"LaneChangeActionDynamics", {"eventbdsj"}},
        {"LaneChangeAction", {"eventzxxpy"}},
        {"SpeedActionDynamics", {"eventjsd"}},
        {"AbsoluteTargetSpeed", {"eventmbsd"}},
        {"AbsoluteTargetLaneOffset", {"eventcdpyl"}},
        {"LaneOffsetActionDynamics", {"eventzdhxjsd"}},
    };
}


bool SaveGeneralizeTp::IsTemplateExist(string _file)
{
    return !access(_file.c_str(), 0);
}

/*!
 * 将泛化的配置保存在模板文件中
 * @param tpd 泛化配置
*/
int SaveGeneralizeTp::SaveDataToXml(const GeneralizeData<GeneralizeTag> &tpd)
{
    //D_PRINTF(D_INFO, "SaveDataToXml start");
    // TGeneralize.xml 本地化配置
    simproxml::XMLDocument doc;
    //添加XML声明
    simproxml::XMLDeclaration *declaration = doc.NewDeclaration();
    doc.LinkEndChild(declaration);
    //doc.LinkEndChild(new simproxml::XMLDeclaration("1.0", "", ""));
    //simproxml::XMLElement *root = new simproxml::XMLElement("TGeneralize");
    simproxml::XMLElement *root = doc.NewElement("TGeneralize");
    doc.LinkEndChild(root);

    //根元素下添加子元素
    simproxml::XMLElement *egoBaseTagList = doc.NewElement("egoBaseTagList");

    root->LinkEndChild(egoBaseTagList);
    for (auto i : tpd.egoBaseTagList)
    {
        simproxml::XMLElement *_tag = doc.NewElement(i.tagName.c_str());
        egoBaseTagList->LinkEndChild(_tag);
        _tag->SetAttribute("begin", std::to_string(i.beginValue).c_str());
        _tag->SetAttribute("end", std::to_string(i.endValue).c_str());
        _tag->SetAttribute("step", std::to_string(i.stepValue).c_str());
    }

    simproxml::XMLElement *egoEventTagMap = doc.NewElement("egoEventTagMap");
    root->LinkEndChild(egoEventTagMap);
    for (auto event : tpd.egoEventTagMap)
    {
        simproxml::XMLElement *egoEventTag = doc.NewElement(event.first.c_str());
        egoEventTagMap->LinkEndChild(egoEventTag);
        for (auto i : event.second)
        {
            simproxml::XMLElement *_tag = doc.NewElement(i.tagName.c_str());
            egoEventTag->LinkEndChild(_tag);
            _tag->SetAttribute("begin", std::to_string(i.beginValue).c_str());
            _tag->SetAttribute("end", std::to_string(i.endValue).c_str());
            _tag->SetAttribute("step", std::to_string(i.stepValue).c_str());
        }
    }

    simproxml::XMLElement *vehicleTagMap = doc.NewElement("vehicleTagMap");
    root->LinkEndChild(vehicleTagMap);
    for (auto vehicle : tpd.vehicleTagMap)
    {
        simproxml::XMLElement *vehicleTag = doc.NewElement("Vehicle");
        vehicleTag->SetAttribute("name", vehicle.first.c_str());
        vehicleTagMap->LinkEndChild(vehicleTag);
        for (auto i : vehicle.second)
        {
            simproxml::XMLElement *_tag = doc.NewElement(i.tagName.c_str());
            vehicleTag->LinkEndChild(_tag);
            _tag->SetAttribute("begin", std::to_string(i.beginValue).c_str());
            _tag->SetAttribute("end", std::to_string(i.endValue).c_str());
            _tag->SetAttribute("step", std::to_string(i.stepValue).c_str());
        }
    }

    simproxml::XMLElement *vehicleEventMap = doc.NewElement("vehicleEventMap");
    root->LinkEndChild(vehicleEventMap);
    for (auto event : tpd.vehicleEventMap)
    {
        simproxml::XMLElement *vehicleEvent = doc.NewElement("vehicleEvent");
        vehicleEvent->SetAttribute("entityRef", event.first.c_str());
        vehicleEventMap->LinkEndChild(vehicleEvent);
        for (auto vehicle : event.second)
        {
            simproxml::XMLElement *eventTag = doc.NewElement(vehicle.first.c_str());
            vehicleEvent->LinkEndChild(eventTag);
            for (auto i : vehicle.second)
            {
                simproxml::XMLElement *_tag = doc.NewElement(i.tagName.c_str());
                eventTag->LinkEndChild(_tag);

                _tag->SetAttribute("begin", std::to_string(i.beginValue).c_str());
                _tag->SetAttribute("end", std::to_string(i.endValue).c_str());
                _tag->SetAttribute("step", std::to_string(i.stepValue).c_str());
            }
        }
    }

    // 行人基本参数列表
    simproxml::XMLElement *pedestrianTagMap = doc.NewElement("pedestrianTagMap");
    root->LinkEndChild(pedestrianTagMap);
    for (auto event : tpd.pedestrianTagMap)
    {
        simproxml::XMLElement *pedestrianTag = doc.NewElement("Pedestrian");
        pedestrianTag->SetAttribute("name", event.first.c_str());
        pedestrianTagMap->LinkEndChild(pedestrianTag);
        for (auto i : event.second)
        {
            simproxml::XMLElement *_tag = doc.NewElement(i.tagName.c_str());
            pedestrianTag->LinkEndChild(_tag);
            _tag->SetAttribute("begin", std::to_string(i.beginValue).c_str());
            _tag->SetAttribute("end", std::to_string(i.endValue).c_str());
            _tag->SetAttribute("step", std::to_string(i.stepValue).c_str());
        }
    }

    //行人关联事件列表
    simproxml::XMLElement *pedestrianEventMap = doc.NewElement("pedestrianEventMap");
    root->LinkEndChild(pedestrianEventMap);
    for (auto event : tpd.pedestrianEventMap)
    {
        simproxml::XMLElement *pedestrianEvent = doc.NewElement("pedestrianEvent");
        pedestrianEvent->SetAttribute("entityRef", event.first.c_str());
        pedestrianEventMap->LinkEndChild(pedestrianEvent);
        for (auto pedestrian : event.second)
        {
            simproxml::XMLElement *eventTag = doc.NewElement(pedestrian.first.c_str());
            pedestrianEvent->LinkEndChild(eventTag);
            for (auto i : pedestrian.second)
            {
                simproxml::XMLElement *_tag = doc.NewElement(i.tagName.c_str());
                eventTag->LinkEndChild(_tag);

                _tag->SetAttribute("begin", std::to_string(i.beginValue).c_str());
                _tag->SetAttribute("end", std::to_string(i.endValue).c_str());
                _tag->SetAttribute("step", std::to_string(i.stepValue).c_str());
            }
        }
    }

    // 障碍物基本参数列表
    simproxml::XMLElement *miscobjectTagMap = doc.NewElement("miscobjectTagMap");
    root->LinkEndChild(miscobjectTagMap);
    for (auto event : tpd.miscobjectTagMap)
    {
        // simproxml::XMLElement *egoEventTag = doc.NewElement(event.first.c_str());
        simproxml::XMLElement *miscObjectTag = doc.NewElement("MiscObject");
        miscObjectTag->SetAttribute("name", event.first.c_str());
        miscobjectTagMap->LinkEndChild(miscObjectTag);
        for (auto i : event.second)
        {
            simproxml::XMLElement *_tag = doc.NewElement(i.tagName.c_str());
            miscObjectTag->LinkEndChild(_tag);
            _tag->SetAttribute("begin", std::to_string(i.beginValue).c_str());
            _tag->SetAttribute("end", std::to_string(i.endValue).c_str());
            _tag->SetAttribute("step", std::to_string(i.stepValue).c_str());
        }
    }

    //环境参数列表
    simproxml::XMLElement *envObjectTagVector = doc.NewElement("envObjectTagVector");
    root->LinkEndChild(envObjectTagVector);
    for (auto i : tpd.envObjectTagVector)
    {
        simproxml::XMLElement *_tag = doc.NewElement(i.tagName.c_str());
        envObjectTagVector->LinkEndChild(_tag);
        _tag->SetAttribute("begin", std::to_string(i.beginValue).c_str());
        _tag->SetAttribute("end", std::to_string(i.endValue).c_str());
        _tag->SetAttribute("step", std::to_string(i.stepValue).c_str());
    }
    filepath = filepath + "/TGeneralize.xml";
    doc.SaveFile(filepath.c_str());
    
    //D_PRINTF(D_INFO, "SaveDataToXml end");
    return 0;
}

double GetDoubleValue(simproxml::XMLElement *element, const char *tag)
{
    double val;
    element->QueryDoubleAttribute(tag, &val);
    return val;
}

/*!
 * 从模板文件中获取泛化参数的配置
 * @param file 模板文件路径
 * @return 泛化参数配置
*/
GeneralizeData<GeneralizeTag> SaveGeneralizeTp::GetDataFromXml(const string &file)
{
    GeneralizeData<GeneralizeTag> _data = {};
    simproxml::XMLDocument doc;
    if (!doc.LoadFile("./output/template/TGeneralize.xml"))
    {
        return _data;
    }

    simproxml::XMLElement *root = doc.RootElement();

    simproxml::XMLElement *xoscFileName = root->FirstChildElement("xoscFileName");
    _data.xoscFileName = string(xoscFileName->Attribute("name"));

    simproxml::XMLElement *egoBaseTagList = root->FirstChildElement("egoBaseTagList");
    for (simproxml::XMLElement *ele = egoBaseTagList->FirstChildElement(); ele != nullptr; ele = ele->NextSiblingElement())
    {

        GeneralizeTag _tag = {ele->Value(), GetDoubleValue(ele, "begin"), GetDoubleValue(ele, "end"), GetDoubleValue(ele, "step")};
        _data.egoBaseTagList.emplace_back(_tag);
    }

    simproxml::XMLElement *egoEventTagMap = root->FirstChildElement("egoEventTagMap");
    for (simproxml::XMLElement *event = egoEventTagMap->FirstChildElement(); event != nullptr; event->NextSiblingElement())
    {
        string eventname = event->Value();
        for (simproxml::XMLElement *ele = event->FirstChildElement(); ele != nullptr; ele->NextSiblingElement())
        {
            GeneralizeTag _tag = {ele->Value(), GetDoubleValue(ele, "begin"), GetDoubleValue(ele, "end"), GetDoubleValue(ele, "step")};
            _data.egoEventTagMap[eventname].emplace_back(_tag);
        }
    }

    simproxml::XMLElement *vehicleTagMap = root->FirstChildElement("vehicleTagMap");
    for (simproxml::XMLElement *vehicle = egoEventTagMap->FirstChildElement(); vehicle != nullptr; vehicle->NextSiblingElement())
    {
        string vehiclename = vehicle->Value();
        for (simproxml::XMLElement *ele = vehicle->FirstChildElement(); ele != nullptr; ele->NextSiblingElement())
        {
            GeneralizeTag _tag = {ele->Value(), GetDoubleValue(ele, "begin"), GetDoubleValue(ele, "end"), GetDoubleValue(ele, "step")};
            _data.egoEventTagMap[vehiclename].emplace_back(_tag);
        }
    }

    simproxml::XMLElement *vehicleEventMap = root->FirstChildElement("vehicleEventMap");
    for (simproxml::XMLElement *event = vehicleEventMap->FirstChildElement(); event != nullptr; event->NextSiblingElement())
    {
        string eventname = event->Value();
        for (simproxml::XMLElement *vehicle = event->FirstChildElement(); vehicle != nullptr; vehicle->NextSiblingElement())
        {
            string vehiclename = vehicle->Value();
            for (simproxml::XMLElement *ele = vehicle->FirstChildElement(); ele != nullptr; ele->NextSiblingElement())
            {
                GeneralizeTag _tag = {ele->Value(), GetDoubleValue(ele, "begin"), GetDoubleValue(ele, "end"), GetDoubleValue(ele, "step")};
                _data.vehicleEventMap[eventname][vehiclename].emplace_back(_tag);
            }
        }
    }

    // 行人基本参数列表
    simproxml::XMLElement *pedestrianTagMap = root->FirstChildElement("pedestrianTagMap");
    for (simproxml::XMLElement *event = pedestrianTagMap->FirstChildElement(); event != nullptr; event->NextSiblingElement())
    {
        string eventname = event->Value();
        for (simproxml::XMLElement *ele = event->FirstChildElement(); ele != nullptr; ele->NextSiblingElement())
        {
            GeneralizeTag _tag = {ele->Value(), GetDoubleValue(ele, "begin"), GetDoubleValue(ele, "end"), GetDoubleValue(ele, "step")};
            _data.pedestrianTagMap[eventname].emplace_back(_tag);
        }
    }

    // 障碍物基本参数列表
    simproxml::XMLElement *miscobjectTagMap = root->FirstChildElement("miscobjectTagMap");
    for (simproxml::XMLElement *event = miscobjectTagMap->FirstChildElement(); event != nullptr; event->NextSiblingElement())
    {
        string eventname = event->Value();
        for (simproxml::XMLElement *ele = event->FirstChildElement(); ele != nullptr; ele->NextSiblingElement())
        {
            GeneralizeTag _tag = {ele->Value(), GetDoubleValue(ele, "begin"), GetDoubleValue(ele, "end"), GetDoubleValue(ele, "step")};
            _data.miscobjectTagMap[eventname].emplace_back(_tag);
        }
    }

    simproxml::XMLElement *envObjectTagVector = root->FirstChildElement("envObjectTagVector");
    for (simproxml::XMLElement *ele = envObjectTagVector->FirstChildElement(); ele != nullptr; ele = ele->NextSiblingElement())
    {

        GeneralizeTag _tag = {ele->Value(), GetDoubleValue(ele, "begin"), GetDoubleValue(ele, "end"), GetDoubleValue(ele, "step")};
        _data.envObjectTagVector.emplace_back(_tag);
    }

    return _data;
}

/*!
 * 获取element标签下，可泛化的标签的内容
 * @param element 标签内容
 * @param isEventExist 是否存在事件 true：是，false：否
 * @param isBaseNote 是否获取基本泛化参数，true：是，false：否
 * @param eventName 事件名称
 * @return 泛化标签列表
*/
vector<GeneralizeTag> SaveGeneralizeTp::GetBaseTag(simproxml::XMLElement *element, bool &isEventExist, bool isBaseNote, char **eventName)
{
    auto _note = _baseNote;
    if (!isBaseNote)
    {
        _note = _relativeNote;
    }
    //使用栈遍历element节点下所有标签
    stack<simproxml::XMLElement *> _st;
    vector<GeneralizeTag> _taglist;
    simproxml::XMLElement *ele = element;
    bool isEventCondition = false;
    bool isTrajectory = false;
    int num = 0, startTriggerNum = 0;

#if 1 //bug13292 zp 20230210
    bool isFindSimulationTimeCondition = false;
#endif

    while (!_st.empty() || ele != nullptr)
    {
        while (ele != nullptr)
        {
           //D_PRINTF(D_INFO, "ele=%s", ele->Value());
            _st.push(ele);
            string eleName = (ele->Value() != nullptr) ? string(ele->Value()) : "";
            if (eleName.compare("Event") == 0)
            {
                isEventExist = true;
                isEventCondition = true;
                *eventName = const_cast<char *>(ele->Attribute("name"));
                //D_PRINTF(D_INFO, "eventName=%s", *eventName);
            }

            if (eleName.compare("ByEntityCondition") == 0)
            {
                isEventCondition = false;
            }  

            if (eleName.compare("Trajectory") == 0)
            {
                isTrajectory = true;
                num++;
            }
            ele = ele->FirstChildElement();
        }
        
        if (!_st.empty())
        {
            ele = _st.top();
            _st.pop();
            string _str = string(ele->Value());
            auto _name = _note.find(_str);
            if (_name != _note.end())
            {
                //D_PRINTF(D_INFO, "_str=%s", _str.c_str());
                for (auto i : _name->second)
                {
                    GeneralizeTag _gt = {i, _range[i][0], _range[i][1], _range[i][2]};

                    if ((_str.compare("SimulationTimeCondition") == 0) && isEventCondition)//关联事件只包含，Event标签下的SimulationTimeCondition
                    {

                        _taglist.push_back(_gt);
                        isEventCondition = false;
#if 1 //bug13292 zp 20230210
                        isFindSimulationTimeCondition = true;
#endif
                    }
                    else
                    {
                        if (_str.compare("SimulationTimeCondition") != 0)
                        {
                            if (_str.compare("LanePosition") == 0 && !isTrajectory)
                            {
                                _taglist.push_back(_gt);
                            }
                            else
                            {
                                if (_str.compare("LanePosition") != 0)
                                {
#if 1 //bug13292 zp 20230210
                                    if(isFindSimulationTimeCondition || _gt.tagName.compare("eventys") != 0)
                                    {
                                        _taglist.push_back(_gt);
                                    }
#else
                                    _taglist.push_back(_gt);
#endif
                                }
                                    
                            }

                            if (num == 2)
                            {
                                isTrajectory = false;
                                num = 0;
                            } 
                        }
                    }
                    
                }
            }

            ele = ele->NextSiblingElement();
            if (_str.compare(string(element->Value())) == 0)
            {
                break;
            }
        }
    }

    return _taglist;
}

/*!
 * 场景文件中所包含的实体名称和其对应的类型
 * @param entitiesEle 场景文件中Entities标签
 * @return 场景文件中包含的实体名称和其对应的类型 <实体名称，类型>
 */
map<string, string> SaveGeneralizeTp::getEntities(simproxml::XMLElement *entitiesEle)
{
    map<string, string> objtype_map;
    for (simproxml::XMLElement *entitySub = entitiesEle->FirstChildElement(); entitySub != nullptr; entitySub = entitySub->NextSiblingElement())
    {
        if (strncmp(entitySub->Value(), "ScenarioObject", 15) == 0)
        {   
            string objname = entitySub->Attribute("name");
            for (simproxml::XMLElement *scenarioSub = entitySub->FirstChildElement(); scenarioSub != nullptr; scenarioSub = scenarioSub->FirstChildElement())
            {
                //场景文件全局配置
                //D_PRINTF(D_INFO, "entitySub=%s", scenarioSub->Value());
                string type = scenarioSub->Value();
                string typeName = type; 

                //场景文件独立配置
                if (type == "CatalogReference")
                {
                    typeName = scenarioSub->Attribute("catalogName"); 
                }

                //D_PRINTF(D_INFO, "typeName=%s", typeName.c_str());
                if (typeName.find("Vehicle") != string::npos && objname.compare("Ego") != 0)
                {
                    //D_PRINTF(D_INFO, "Vehicle = %s", objname.c_str());
                    objtype_map[objname] = "Vehicle";
                    break;
                }
                else if (typeName.find("Pedestrian") != string::npos)
                {
                    //D_PRINTF(D_INFO, "Pedestrian = %s", objname.c_str());
                    objtype_map[objname] = "Pedestrian";
                    break;
                }
                else if (typeName.find("MiscObject") != string::npos)
                {
                    //D_PRINTF(D_INFO, "MiscObject = %s", objname.c_str());
                    objtype_map[objname] = "MiscObject";
                    break;
                }
            }
        }
    }
    return objtype_map;
}
#include <iostream>
vector<GeneralizeTag> SaveGeneralizeTp::GetBaseTagTmp(simproxml::XMLElement *element, bool &isEventExist, bool isBaseNote, char **eventName)
{
    auto _note = _baseNote;
    if (!isBaseNote)
    {
        _note = _relativeNote;
    }
    //使用栈遍历element节点下所有标签
    stack<simproxml::XMLElement *> _st;
    vector<GeneralizeTag> _taglist;
    simproxml::XMLElement *ele = element;
    bool isEventCondition = false;
    bool isTrajectory = false;
    int num = 0, startTriggerNum = 0;;

    while (!_st.empty() || ele != nullptr)
    {
        while (ele != nullptr)
        {
           //D_PRINTF(D_INFO, "ele=%s", ele->Value());
            _st.push(ele);
            string eleName = (ele->Value() != nullptr) ? string(ele->Value()) : "";
            if (eleName.compare("Event") == 0)
            {
                isEventExist = true;
                isEventCondition = true;
                *eventName = const_cast<char *>(ele->Attribute("name"));
                //D_PRINTF(D_INFO, "eventName=%s", *eventName);
            }

            if (eleName.compare("ByEntityCondition") == 0)
            {
                isEventCondition = false;
            }  

            if (eleName.compare("Trajectory") == 0)
            {
                isTrajectory = true;
                num++;
            }
            ele = ele->FirstChildElement();
        }
        
        if (!_st.empty())
        {
            ele = _st.top();
            _st.pop();
            string _str = string(ele->Value());
            auto _name = _note.find(_str);
            if (_name != _note.end())
            {
                std::cout << "_str=" << _str << std::endl;
                for (auto i : _name->second)
                {
                    GeneralizeTag _gt = {i, _range[i][0], _range[i][1], _range[i][2]};

                    if ((_str.compare("SimulationTimeCondition") == 0) && isEventCondition)//关联事件只包含，Event标签下的SimulationTimeCondition
                    {

                        _taglist.push_back(_gt);
                        isEventCondition = false;
                    }
                    else
                    {
                        if (_str.compare("SimulationTimeCondition") != 0)
                        {
                            if (_str.compare("LanePosition") == 0 && !isTrajectory)
                            {
                                _taglist.push_back(_gt);
                            }
                            else
                            {
                                std::cout << "-----" << _gt.tagName << "," << _gt.beginValue << "," << _gt.endValue << std::endl;
                                if (_str.compare("LanePosition") != 0)
                                    _taglist.push_back(_gt);
                            }

                            if (num == 2)
                            {
                                isTrajectory = false;
                                num = 0;
                            } 
                        }
                    }
                    
                }
            }

            ele = ele->NextSiblingElement();
            if (_str.compare(string(element->Value())) == 0)
            {
                break;
            }
        }
    }

    return _taglist;
}

void SaveGeneralizeTp::testGetBaseTag(const string &xoscfile)
{
    simproxml::XMLDocument doc;
    //载入文件
    doc.LoadFile(xoscfile.c_str());
    simproxml::XMLElement *root = doc.RootElement();
    if (root == NULL)
    {
        return ;
    } 
    simproxml::XMLElement *story = root->FirstChildElement("Storyboard");
    simproxml::XMLElement* element = story->FirstChildElement("Story");
    element = element->FirstChildElement("Act");
    element = element->FirstChildElement("ManeuverGroup");

    char *event_name = nullptr;
    bool isEventExist = false;
    vector<GeneralizeTag> _tmplist = GetBaseTagTmp(element, isEventExist, false, &event_name);

    for(int i = 0; i < _tmplist.size(); ++i)
    {
        std::cout << "\t" << _tmplist[i].tagName << "," << _tmplist[i].beginValue << "," << _tmplist[i].endValue << std::endl;
    }
}

/*!
 * 场景文件中包含的可泛化的标签内容
 * @param xoscfile 场景文件路径
 * @return 可泛化标签内容
 * 
*/
GeneralizeData<GeneralizeTag> SaveGeneralizeTp::CurXoscUseTemplate(const string &xoscfile)
{
    GeneralizeData<GeneralizeTag> tpd;
    vector<set<tuple<string, string, string, string, string>>> _entities;

    simproxml::XMLDocument doc;

    int version = 1; // ScenarioManagerBase::getXoscVersion(xoscfile.c_str());

    //载入文件
    doc.LoadFile(xoscfile.c_str());
    tpd.xoscFileName = xoscfile;
    simproxml::XMLElement *root = doc.RootElement();

    if (root == NULL)
    {
        return tpd;
    } 

    simproxml::XMLElement *roadNetwork = root->FirstChildElement("RoadNetwork");
    //地图文件
    simproxml::XMLElement *logicFile = roadNetwork->FirstChildElement("LogicFile");
    auto roadFile = logicFile->Attribute("filepath");
    if (roadFile != nullptr)
    {
        tpd.logicFileName = roadFile;
    }

    //可视化数据库
    simproxml::XMLElement *sceneGraphFile = roadNetwork->FirstChildElement("SceneGraphFile");
    auto osgbFile = sceneGraphFile->Attribute("filepath");
    if (osgbFile != nullptr)
    {
        tpd.sceneGraphFileName = osgbFile;
    }

    simproxml::XMLElement *entities = root->FirstChildElement("Entities");
    map<string, string> objtype_map = getEntities(entities);

    simproxml::XMLElement *story = root->FirstChildElement("Storyboard");
    //遍历基本参数
    simproxml::XMLElement *init = story->FirstChildElement("Init");
    simproxml::XMLElement *action = init->FirstChildElement("Actions");

    simproxml::XMLElement *element = action->FirstChildElement("Private");
    set<string> _entity;

    while (strncmp(element->Value(), "Private", 8))
    {
        //D_PRINTF(D_INFO, "element name=%s", element->Value());
        element = element->FirstChildElement();
    }

    for (; element != nullptr; element = element->NextSiblingElement())
    {
        string entityref = (element->Attribute("entityRef") != nullptr) ? string(element->Attribute("entityRef")) : "";
        bool isEventExist;

        //D_PRINTF(D_INFO, "entityref name=%s", entityref.c_str());
        if (entityref.compare("Ego") == 0)
        {
            tpd.egoBaseTagList = GetBaseTag(element, isEventExist);
        }
        else
        {
            if (objtype_map.find(entityref) != objtype_map.end())
            {
                if (objtype_map.at(entityref) == "Vehicle")
                {
                    vector<GeneralizeTag> _tmp = GetBaseTag(element, isEventExist);
                    tpd.vehicleTagMap.emplace(entityref, _tmp);
                }
                else if (objtype_map.at(entityref) == "Pedestrian")
                {
                    vector<GeneralizeTag> _tmp = GetBaseTag(element, isEventExist);
                    tpd.pedestrianTagMap.emplace(entityref, _tmp);
                }
                else if (objtype_map.at(entityref) == "MiscObject")
                {
                    vector<GeneralizeTag> _tmp = GetBaseTag(element, isEventExist);
                    tpd.miscobjectTagMap.emplace(entityref, _tmp);
                }
            }
        }        
    }

    //遍历关联事件
    element = story->FirstChildElement("Story");
    element = element->FirstChildElement("Act");
    //element = element->FirstChildElement("ManeuverGroup");

    for (; element != nullptr; element = element->NextSiblingElement())
    {
        char *event_name = nullptr;
        bool isEventExist = false;
        simproxml::XMLElement * elementbak = element;
        element = element->FirstChildElement("ManeuverGroup");
        vector<GeneralizeTag> _tmplist = GetBaseTag(element, isEventExist, false, &event_name);
        element = elementbak;
        string eventName = (event_name != nullptr) ? string(event_name) : "";
        
        // 遍历事件中车辆
        simproxml::XMLElement *_tmpelement = element;

        //D_PRINTF(D_INFO, "_tmpelement->Value=%s", _tmpelement->Value());
        while (strncmp("EntityRef", _tmpelement->Value(), 10) != 0)
        {
            _tmpelement = _tmpelement->FirstChildElement();
            if (_tmpelement == nullptr)
            {
                break;
            }
        }

        if (isEventExist) //Event标签存在时，才会存在关联事件
        {
            for (; _tmpelement != nullptr; _tmpelement = _tmpelement->NextSiblingElement())
            {
                string _name = (_tmpelement->Attribute("entityRef") != nullptr) ? std::string(_tmpelement->Attribute("entityRef")) : "";

                if (_name.compare("Ego") == 0)
                {
                    tpd.egoEventTagMap.emplace(eventName, _tmplist);
                }
                else
                {
                    if (objtype_map.find(_name) != objtype_map.end())
                    {
                        if (objtype_map.at(_name) == "Vehicle")
                        {
                            tpd.vehicleEventMap[_name].emplace(eventName, _tmplist);
                        }
                        else if (objtype_map.at(_name) == "Pedestrian")
                        {
                            tpd.pedestrianEventMap[_name].emplace(eventName, _tmplist);
                        }
                    }
                }
            }
        }
        
    }

    simproxml::XMLElement *globalaction = action->FirstChildElement("GlobalAction");
    if (globalaction != nullptr)
    {
        element = globalaction->FirstChildElement("EnvironmentAction");
        element = element->FirstChildElement("Environment");
        // 遍历环境 重复代码较多 考虑优化
        if (element != nullptr)
        {
            // 日期
            simproxml::XMLElement *_tmpele = element->FirstChildElement("TimeOfDay");
            if (_tmpele != nullptr)
            {
                GeneralizeTag _tmptag = {"Time", _range["Time"][0], _range["Time"][1], _range["Time"][2]};
                tpd.envObjectTagVector.push_back(_tmptag);
            }

            // 雨 雪
            simproxml::XMLElement *_weather = element->FirstChildElement("Weather");
            _tmpele = _weather->FirstChildElement("Precipitation");
            if (_tmpele != nullptr)
            {
                string _precipitationType = _tmpele->Attribute("precipitationType");
                if ("rain" == _precipitationType)
                {
                    GeneralizeTag _tmptag = {"Rain", _range["Rain"][0], _range["Rain"][1], _range["Rain"][2]};
                    tpd.envObjectTagVector.push_back(_tmptag);
                }

                if ("snow" == _precipitationType)
                {
                    GeneralizeTag _tmptag = {"Snow", _range["Snow"][0], _range["Snow"][1], _range["Rain"][2]};
                    tpd.envObjectTagVector.push_back(_tmptag);
                }
            }
  
            // 雾
            
            if (_tmpele != nullptr)
            {
                _tmpele = _weather->FirstChildElement("Fog");
                if (_tmpele != nullptr)
                {
                    GeneralizeTag _tmptag = {"Fog", _range["Fog"][0], _range["Fog"][1], _range["Fog"][2]};
                    tpd.envObjectTagVector.push_back(_tmptag);
                }
            }

            // 摩擦系数
            _tmpele = element->FirstChildElement("RoadCondition");
            if (_tmpele != nullptr)
            {
                GeneralizeTag _tmptag = {"Friction", _range["Friction"][0], _range["Friction"][1], _range["Rain"][2]};
                tpd.envObjectTagVector.push_back(_tmptag);
            }
        }
    }
    

    return tpd;
}

void SaveGeneralizeTp::SetFilePath(string path)
{
    filepath = path;
}
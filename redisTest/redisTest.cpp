#include <unistd.h>
#include <chrono>
#include <tuple>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <sw/redis++/redis++.h>
#include <sw/redis++/sentinel.h>
#include <sw/redis++/connection.h>
#include <sw/redis++/connection_pool.h>
//using namespace std;
using namespace sw::redis;
using namespace std::chrono;
int main()
{
    ConnectionOptions connection_options;
    connection_options.host = "127.0.0.1";  // Required.
    connection_options.port = 6379; // Optional. The default port is 6379.
    //connection_options.password = "auth";   // Optional. No password by default.
    connection_options.db = 5;  // Optional. Use the 0th database by default.
 
    ConnectionPoolOptions pool_options;
    pool_options.size = 3;  // Pool size, i.e. max number of connections.
    pool_options.wait_timeout = std::chrono::milliseconds(100);
 
    ConnectionOptions connection_options2;
    connection_options2.host = "127.0.0.1"; 
    connection_options2.port = 6379;
    connection_options2.db = 7;
 
    ConnectionPoolOptions pool_options7;
    pool_options7.size = 3; 
    pool_options7.wait_timeout = std::chrono::milliseconds(100);
 
    Redis * redisofDB1 = NULL;
    Redis * redisofDB7 = NULL;
    // 开始连接
    try{
        redisofDB1 = new Redis(connection_options, pool_options);
    
        redisofDB7 = new Redis(connection_options2, pool_options7);
    }catch (const ReplyError &err) {
        printf("RedisHandler-- ReplyError：%s \n",err.what());
        return false ;
    }catch (const TimeoutError &err) {
        printf("RedisHandler-- TimeoutError%s \n",err.what());
        return false ;
    }catch (const ClosedError &err) {
        printf("RedisHandler-- ClosedError%s \n",err.what());
        return false ;
    }catch (const IoError &err) {
        printf("RedisHandler-- IoError%s \n",err.what());
        return false ;
    }catch (const Error &err) {
        printf("RedisHandler-- other%s \n",err.what());
        return false ;
    }

    

    std::cout << "over test." << std::endl;
 
    /*
    std::map<std::string, std::string> hashTerm;
    redisofDB7->hgetall("FORWARD.PLAT.DETAIL",std::inserter(hashTerm, hashTerm.end()));
    
    
    for(auto it1 = hashTerm.begin() ;it1 != hashTerm.end(); it1++)
    {
        std::cout <<"Plat ID："  <<it1->first <<std::endl;
        std::cout <<  "Plat UserName & Password"<<it1->second <<std::endl;
    }
    */
 #if 1
    // 开始干活
    auto cursor = 0LL;
    auto pattern = "*";
    auto count = 5;
    //std::unordered_set<std::string> keys;
    std::map<std::string, std::string> hashs;
    while (true) {
        cursor = redisofDB7->hscan("FORWARD.PLAT.DETAIL",cursor, pattern, count, std::inserter(hashs, hashs.begin()));
 
        if (cursor == 0) {
            break;
        }
    }
    if(hashs.size() < 1)
    {
        printf("we get nothing !\n");
    }
    for(auto it1 = hashs.begin() ;it1 != hashs.end(); it1++)
    {
        std::cout <<"Plat ID："  <<it1->first <<std::endl;
        std::cout <<  "Plat UserName & Password"<<it1->second <<std::endl;
    }
 
    OptionalString strValue = redisofDB1->hget("XNY.CARINFO","CRC01211711100232");
    std::cout<< " CRC01211711100232  的 vin :" << *strValue << std::endl;
    std::string straa = *strValue;
    if(straa.empty())
    {
           std::cout << "we gete nothing " << std::endl ;
    }
    std::cout<< " ---- CRC01211711100232  的 details :" << straa << std::endl;
 
    std::cout<< " ---- 下面试试hincrby ---- " << std::endl;
 
 
 
    auto cursor2 = 0LL;
    auto pattern2 ="*";
    auto count2 = 20;
    std::map<std::string, std::string> vv;
    std::vector<std::string> vlist;
    while (true) {
        cursor2 = redisofDB7->hscan("FORWARD.LIST.002",cursor2, pattern2, count2, std::inserter(vv, vv.begin()));
 
        if (cursor2 == 0) {
            break;
        }
    }
 
    for(auto it1 = vv.begin() ;it1 != vv.end(); it1++)
    {
        vlist.push_back(it1->first);
    }
 
    for(auto uu = vlist.begin(); uu !=vlist.end(); uu ++ )
    {
        std::cout << *uu << std::endl;
    }
 #endif 
    return 0;
}
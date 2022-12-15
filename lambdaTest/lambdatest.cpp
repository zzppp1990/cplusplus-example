/*
*语法：[ capture ] ( params ) opt -> ret { body; };
* capture 是捕获列表，params 是参数表，opt 是函数选项，ret 是返回值类型，body是函数体
* 
* 参考：http://c.biancheng.net/view/3741.html
*/

#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> v = { 1, 2, 3, 4, 5, 6 };
    int even_count = 0;
    for_each( v.begin(), v.end(), [&even_count](int val)
        {
            if (!(val & 1))  // val % 2 == 0
            {
                ++ even_count;
            }
        });
    std::cout << "The number of even is " << even_count << std::endl;
    return 0;
}
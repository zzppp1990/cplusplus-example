#include<iostream>
#include<fstream>
using namespace std;

#define FILE_NAME "getput.txt"

/*
* ofstream 写文件
*/
void writeFile() {
    ofstream ofs(FILE_NAME);
    if(!ofs){
        perror("打开文件失败！");
        return;
    }
    ofs << "this a fstream test." << endl;
    ofs.close();
}

/*
* ifstream 读文件
*/
void readFile() {
    ifstream ifs(FILE_NAME);
    char c;
    while((c = ifs.get()) != EOF)
    {
        cout<<c;
    }

    if(! ifs.eof())
    {
        perror("读取文件失败1");
        return;
    }
    ifs.close();
}

int main()
{
    writeFile();
    readFile();
    return 0;
}


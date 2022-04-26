#include <iostream>
using namespace std;

class CStudent {
public:
    string m_sName;

public:
    CStudent(string& inStr) : m_sName(inStr) {}
    void Print() {
        cout << "student name is " << m_sName << endl;
    }

    string& getName() {
        return m_sName;
    }
};

int main()
{
    string sName = "zzpp";
    CStudent s(sName);
    s.Print();
    
    string CStudent::*pname = &CStudent::m_sName;
    cout << s.*pname << endl;
    
    string& t_sName = s.getName();
    cout << t_sName << endl;

    return 0;
}


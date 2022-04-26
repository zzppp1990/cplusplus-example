/*
*两种实现方式
*类成员函数方式
*全局函数方式，使用friend，友元函数是全局的
*/

#include<iostream>
#define MATRIX_LEN 3

//矩阵实现
class CMatrix
{
private:
    int m_arr[MATRIX_LEN][MATRIX_LEN];
    int m_iData;
public:
    CMatrix(int data)
    {
        m_iData = data;
        int i,j;
        for(i=0;i<MATRIX_LEN;i++)
            for(j=0;j<MATRIX_LEN;j++)
            {
                if(i == j)
                    m_arr[i][j] =data;
                else 
                   m_arr[i][j] = 0;
            }
    }

    const CMatrix operator+(const CMatrix& m)const
    {
        return CMatrix(m_iData+m.m_iData);
    }

    const CMatrix operator*(const CMatrix& m)const
    {
        return CMatrix(m_iData*m.m_iData);
    }

    const CMatrix& operator+=(const CMatrix& m)
    {
        m_iData += m.m_iData;
        *this = CMatrix(m_iData);
        return *this;
    }

    const CMatrix& operator*=(const CMatrix& m)
    {
        m_iData *=m.m_iData;
        *this = CMatrix(m_iData);
        return *this;
    }

    CMatrix& operator++()
    {
        m_iData++;
        *this = CMatrix(m_iData);
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os,const CMatrix& m)
    {
        int i,j;
        for(i=0;i<MATRIX_LEN;i++)
        {
            for(j=0;j<MATRIX_LEN;j++)
                os<<m.m_arr[i][j]<<"  ";
            os<<std::endl;
        }
        os<<std::endl;
        return os;
    }
};

int main()
{
    std::cout<< "m1 : "<< std::endl;
    CMatrix m1(1);
    std::cout<<m1;

    std::cout<< "m2 = m1 + m1,m2 : "<< std::endl;
    CMatrix m2 = m1+m1;
    std::cout<<m2;

    std::cout<< "m2 += m1, m2: "<< std::endl;
    m2 += m1;
    std::cout<<m2;

    std::cout<< "m2 *= m1, m2: "<< std::endl;
    m2 *= m1;
    std::cout<<m2;

    std::cout<< "m3 = ++m1, m3: "<< std::endl;
    CMatrix m3 = ++m1;
    std::cout<<m3;

    return 0;
}


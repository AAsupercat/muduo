#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <iostream>
class MyString
{
private:
    char *_data;
    size_t _size;

    void copy_from(const char *str)
    {
        if (str == nullptr)
        {
            _size = 0;
            _data = new char[1];
            _data[0] = '\0';
        }
        else
        {
            _size = strlen(str);
            _data = new char[_size + 1];
            strcpy(_data, str); // 拷贝前注意空间
        }
        return;
    }

public:
    MyString() : _data(new char[1]), _size(0)
    {
        _data[0] = '\0';
    }
    // c语言式赋值
    MyString(const char *str)
    {
        copy_from(str);
    }
    // 拷贝构造(深拷贝)
    MyString(const MyString &other)
    {
        _size = other._size;
        _data = new char[_size + 1]; // 这里是构造函数
        std::strcpy(_data, other._data);
    }
    // 移动拷贝
    MyString(MyString &&other) noexcept
        : _data(other._data), _size(other._size)
    {
        // 置空后释放
        other._data = nullptr;
        other._size = 0;
    }

    ~MyString()
    {
        delete[] _data;
        _data = nullptr;
    }

    // MyString& operator=(const MyString& other)
    // {
    //  先delete释放，要是new开辟空间失败（内存不足）那原本的this就修改了
    //     if(this != &other)
    //     {
    //         delete[] _data;
    //         _size=other._size;
    //         _data=new char[_size+1];
    //         strcpy(_data,other._data);
    //     }

    //     return *this;
    // }

    MyString &operator=(const MyString &other)
    {
        if (this != &other)
        {
            char *new_data = new char[other._size + 1];
            strcpy(new_data,other._data);

            delete[] _data;
            _data = new_data;
            _size = other._size;
        }
        return *this;
    }
    // MyString &operator=(MyString &&other) noexcept
    // {
    //     if (this != &other)
    //     {
    //         delete[] _data;
    //         _data = other._data;
    //         _size = other._size;

    //         other._data = nullptr;
    //         other._size = 0;
    //     }
    //     return *this;
    // }
    //移动语义 赋值
    MyString &operator=(MyString &&other) noexcept
    {
        swap(*this,other);
        return *this;
    }

    friend void swap(MyString& s1,MyString& s2) noexcept
    {
        using std::swap; //利用ADL，查找最优秀的调用
        swap(s1._data,s2._data);
        swap(s1._size,s2._size);
        return ;
    }
    //获取C风格字符串
    const char* c_str() const {return _data;}

    //获取字符数量
    size_t size() const {return _size;}

    //可读下标访问
    char operator[](size_t index) const
    {
        if(index>_size) throw std::out_of_range("Index out of range");
        return _data[index];
    }

    // 可写下标访问
    char& operator[](size_t idx) {
        if (idx > _size) throw std::out_of_range("Index out of range");
        return _data[idx];
    }

    MyString& operator+=(const MyString& other)
    {
        //开辟一块很大的空间
        size_t new_size=_size+other._size;
        char* new_data=new char[new_size+1];

        strcpy(new_data,_data);
        strcpy(new_data+_size,other._data);

        delete[] _data;
        _data=new_data;
        _size=new_size;
        return *this;
    }

    MyString& operator+=(const char* str)
    {
        if(str==nullptr) return *this;
        size_t new_size =_size+strlen(str);
        char* new_data=new char[new_size+1];

        strcpy(new_data,_data);
        strcpy(new_data+_size,str);

        delete[] _data;
        _data=new_data;
        _size=new_size;
        return *this;
    }

};

MyString operator+(const MyString& lhs,const MyString& rhs)
{
    MyString result=lhs;
    result+=rhs;
    return result;
}

std::ostream& operator<<(std::ostream& os,const MyString& str )
{
    os << str.c_str();
    return os;
}

int main()
{
    MyString str1("hello");
    MyString str2=str1;
    MyString str3;
    str3+=str1;
    str3+=str1;


    std::cout<<str1<<std::endl;
    std::cout<<str2<<std::endl;
    std::cout<<str3<<std::endl;

    str3=str2;
    std::cout<<str3<<std::endl;

    return 0;
}
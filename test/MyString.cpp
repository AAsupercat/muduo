#include <vector>

#include <iostream>
#include <cstring>      // strlen, strcpy, etc.
#include <algorithm>    // std::copy
#include <stdexcept>

class MyString {
private:
    char* data_;
    size_t size_;

    // 工具函数：分配并复制 C 字符串
    void copy_from(const char* str) {
        if (!str) {
            size_ = 0;
            data_ = new char[1];
            data_[0] = '\0';
        } else {
            size_ = std::strlen(str);   //这里并没有统计/0
            data_ = new char[size_ + 1];
            std::strcpy(data_, str);    //拷贝
        }
    }

public:
    // 默认构造函数：空字符串 ""
    MyString() : data_(new char[1]), size_(0) {
        data_[0] = '\0';
    }

    // C 风格字符串构造
    MyString(const char* str) {
        copy_from(str);
    }

    // 拷贝构造函数（深拷贝！）
    MyString(const MyString& other) : size_(other.size_) {
        data_ = new char[size_ + 1];
        std::strcpy(data_, other.data_);
    }

    // 移动构造函数（C++11）
    MyString(MyString&& other) noexcept 
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;   // 将原对象置空
        other.size_ = 0;
    }

    // 析构函数
    ~MyString() {
        delete[] data_;
    }

    // 拷贝赋值运算符（异常安全：先 new 再 delete）
    MyString& operator=(const MyString& other) {
        if (this != &other) {
            char* new_data = new char[other.size_ + 1];
            std::strcpy(new_data, other.data_);

            delete[] data_;       // 先释放旧资源
            data_ = new_data;
            size_ = other.size_;
        }
        return *this;
    }

    // 移动赋值运算符
    MyString& operator=(MyString&& other) noexcept {
        if (this != &other) {
            delete[] data_;       // 释放当前资源

            data_ = other.data_;
            size_ = other.size_;

            other.data_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    // 获取 C 风格字符串
    const char* c_str() const { return data_; }

    // 获取长度
    size_t size() const { return size_; }

    // 只读下标访问
    char operator[](size_t idx) const {
        if (idx > size_) throw std::out_of_range("Index out of range");
        return data_[idx];
    }

    // 可写下标访问
    char& operator[](size_t idx) {
        if (idx > size_) throw std::out_of_range("Index out of range");
        return data_[idx];
    }

    // += 运算符（追加另一个 MyString）
    MyString& operator+=(const MyString& other) {
        size_t new_size = size_ + other.size_;
        char* new_data = new char[new_size + 1];
        
        std::strcpy(new_data, data_);
        std::strcpy(new_data + size_, other.data_);

        delete[] data_;
        data_ = new_data;
        size_ = new_size;
        return *this;
    }

    // += 运算符（追加 C 字符串）
    MyString& operator+=(const char* str) {
        if (!str) return *this;
        size_t add_len = std::strlen(str);
        size_t new_size = size_ + add_len;
        char* new_data = new char[new_size + 1];

        std::strcpy(new_data, data_);
        std::strcpy(new_data + size_, str);

        delete[] data_;
        data_ = new_data;
        size_ = new_size;
        return *this;
    }
};

// 全局 + 运算符（非成员函数）
MyString operator+(const MyString& lhs, const MyString& rhs) {
    MyString result = lhs;
    result += rhs;
    return result;
}

// 支持 cout 输出
std::ostream& operator<<(std::ostream& os, const MyString& str) {
    os << str.c_str();
    return os;
}
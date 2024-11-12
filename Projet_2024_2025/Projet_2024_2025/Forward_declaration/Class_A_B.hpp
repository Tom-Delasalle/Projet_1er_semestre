#pragma once
#include <string>

class A{
    public:
    std::string who_i_am(){
        return "I am A";
    }
};

class B{
    std::string who_i_am(){
        return "I am B";
    }
};

template <typename _T>
class A_t{
    _T t;
};

template <typename _T>
class B_t{
    _T t;
};
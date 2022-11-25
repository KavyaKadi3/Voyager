#include "stringOp.h"

std::string removeWhiteSpace(std::string str){
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
    return str;
}
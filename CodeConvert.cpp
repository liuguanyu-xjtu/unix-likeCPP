//
//  CodeConvert.cpp
//  CPP1
//
//  Created by 刘冠宇 on 17/6/17.
//  Copyright © 2017年 刘冠宇. All rights reserved.
//

#include "CodeConvert.hpp"
#include <string.h>
#include <stdlib.h>
#define UTF8_LEN 6
CodeConvert::CodeConvert(const char *from,const char *to):
    type(iconv_open(to, from))

{
    
}
CodeConvert::~CodeConvert(){
    iconv_close(type);
}
/*
 return a c-string in heap ,you need to deallocate it manually
 */
char * CodeConvert::convert(const char *p,const size_t len,size_t *result_len) const{
    char *tmp = (char *)malloc(UTF8_LEN*len);
    char *ptr1 = const_cast<char *>(p),*ptr2=tmp;
    size_t len1=len,len2=(UTF8_LEN*len);
    if(iconv(this->type, (char **)&ptr1, &len1, (char **)&ptr2, &len2)!=0){
        return NULL;
    }
    *result_len = strlen(tmp);
    char *result = (char *)malloc(*result_len+1);
    memcpy(result, tmp, *result_len);
    result[*result_len]=0;
    free(tmp);
    return result;
}
bool CodeConvert::convert(const char *p,const size_t len,std::string &str) const{
    char *tmp = (char *)malloc(UTF8_LEN*len);
    char *ptr1 = const_cast<char *>(p),*ptr2=tmp;
    size_t len1=len,len2=(UTF8_LEN*len);
    if(iconv(this->type, (char **)&ptr1, &len1, (char **)&ptr2, &len2)!=0){
        return false;
    }
//    *result_len = strlen(tmp);
    str = tmp;
    free(tmp);
    return true;
    
    
}
std::string CodeConvert::convert(const char *p,const size_t len) const{
    char *tmp = (char *)malloc(UTF8_LEN*len);
    char *ptr1 = const_cast<char *>(p),*ptr2=tmp;
    size_t len1=len,len2=(UTF8_LEN*len);
    if(iconv(this->type, (char **)&ptr1, &len1, (char **)&ptr2, &len2)!=0){
        return std::string("");
    }
    free(tmp);
    return std::string(tmp);
    
    
    
}
bool CodeConvert::convert(const char *p,const size_t len,char *r,size_t rl) const{
    char *ptr1 = const_cast<char *>(p),*ptr2=r;
    size_t len1=len,len2=rl;
    if(iconv(this->type, (char **)&ptr1, &len1, (char **)&ptr2, &len2)!=0){
        return false;
    }
    return true;
}

//char * CodeConvert::convert( std::string &p,size_t *result_len){
//    return this->convert(p.c_str(), p.length(), result_len);
//}
//bool CodeConvert::convert( std::string &p,std::string &str){
//    return this->convert(p.c_str(), p.length(), result_len);
//}
//std::string CodeConvert::convert(std::string &p,const size_t len){
//    
//}

std::string CodeConvert::convert(const std::string &p) const{
    size_t s = p.size();
    size_t l1,l2;
    char *ptr = const_cast<char *>(p.c_str());
    return this->convert(ptr,p.size() );
    
}

#undef UTF8_LEN

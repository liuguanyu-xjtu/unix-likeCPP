//
//  CodeConvert.hpp
//  CPP1
//
//  Created by 刘冠宇 on 17/6/17.
//  Copyright © 2017年 刘冠宇. All rights reserved.
//

#ifndef CodeConvert_hpp
#define CodeConvert_hpp
#include <iconv.h>
#include <stdio.h>
#include <iostream>
class CodeConvert{
protected:
    iconv_t type;
public:
    /*
     the from and to , you should type 'man 3 iconv_open' to look 
     up the legal defination.
     The most common is GBK UTF-8
     
     be careful that the third function allocate memory in heap you should free it to avoid memory leak
     All functions allocate temopary memory in stack first. In common case ,they won't case stack overflow. In some extreme case they will allocate memory in heap.
     
     */
    
    
    CodeConvert(const char *from,const char *to);
    virtual ~CodeConvert();
    char * convert(const  char *p,const size_t len,size_t *result_len) const;
    bool convert( const char *p,const size_t len,std::string &str) const;
    std::string convert(const char *p,const size_t len) const;
    bool convert(const char *p,const size_t len,char *r,size_t rl) const;
    std::string convert(const std::string &p) const;
    
};



#endif /* CodeConvert_hpp */

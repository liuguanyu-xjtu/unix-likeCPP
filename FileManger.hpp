//
//  FileManger.hpp
//  CPP1
//
//  Created by 刘冠宇 on 17/6/15.
//  Copyright © 2017年 刘冠宇. All rights reserved.
//

#ifndef FileManger_hpp
#define FileManger_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;


class File{
public:
    File();
    File(const std::string &path);
    File(const std::string &parent,const std::string &name);
    std::string getName();
    std::string getPath();
    std::string getAbsolutePath();
    bool exits();
//File type
    bool isDir();
    bool isRegularFile();
    bool isFifo();
    bool isLink();
//permission
    bool isReadable();
    bool isWritable();
    bool isExecutable();
//dir operation
    //-1 means not able to list.Maybe it's not a director,return count
    long listFiles(std::vector<File> &files);
    bool contains(const File &f);
    
    
    bool mkdir(const std::string &name);
    
    bool deleteFile(bool force);
    
    virtual ~File();

protected:
    mode_t getFileMode();
    mode_t file_mode;
    std::string path,parent,name;
    
};




#endif /* FileManger_hpp */

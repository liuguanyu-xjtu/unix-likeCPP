//
//  FileManger.cpp
//  CPP1
//
//  Created by 刘冠宇 on 17/6/15.
//  Copyright © 2017年 刘冠宇. All rights reserved.
//
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include "FileManger.hpp"
File::File():path("/"),parent("/"),name("/"){

}
File::File(const std::string &_path):path(_path){
    if(path[0]!='/'){
        char buf[10000];
        string cwd = getcwd(buf,1000);
        path = cwd+'/'+path;
        
    }
    long len = path.size();
    long tmp=-1;
    for(tmp=len-1;tmp>=0;tmp--){
        if(path[tmp]=='/'){
            break;
        }
    }
    name = path.substr(tmp+1);
    parent = path.substr(0,tmp-1);
    getFileMode();
    return;
    
}
File::File(const std::string &parent,const std::string &name){
    this->path = parent+"/"+name;
    this->name = name;
    this->parent = parent;
    getFileMode();
    return;
    
}
bool File::exits(){
    return access(path.c_str(),F_OK )==0;
}
std::string File::getName(){
    return name;
}
std::string File::getPath(){
    return parent;
}
std::string File::getAbsolutePath(){
    return this->path;
}
//文件文件夹
bool File::isDir(){
    return S_ISDIR(file_mode)!=0;
}
bool File::isLink(){
    return S_ISLNK(file_mode)!=0;
}
bool File::isRegularFile(){
return S_ISREG(file_mode)!=0;
}
bool File::isFifo(){
    return S_ISFIFO(file_mode)!=0;
}
//权限
bool File::isReadable(){
    return access(path.c_str(),R_OK )==0;
}
bool File::isWritable(){

    return access(path.c_str(),W_OK )==0;
}
bool File::isExecutable(){
    return access(path.c_str(),X_OK )==0;
}
//文件夹
long File::listFiles(std::vector<File> &files){
    int count = -1;
    if(!this->isDir()||!this->isReadable()){
        return -1;
    }
    cout<<path.c_str()<<endl;
    DIR *dir=opendir(path.c_str());
    dirent *tmp = NULL;
    count=0;
    while((tmp=readdir(dir))!=NULL){
        if(strcmp(tmp->d_name,".")==0){
            continue;
        }
        if(strcmp(tmp->d_name,"..")==0){
            continue;
        }
        count++;
        files.push_back(File(this->path,tmp->d_name));
    }
    
    closedir(dir);
    return count;
}
bool File::contains(const File &f){
//    f.path.find(path)==0
    return f.path.find(path)==0;
}
File::~File(){}
mode_t File::getFileMode(){
    struct stat stat1;
    int  f = stat(path.c_str(), &stat1);
    if(f!=0){
        return 0;
    }
    mode_t t = stat1.st_mode;
    this->file_mode = t;
    return t;
}




bool File::mkdir(const std::string &name){
    char buf[2048];
    sprintf(buf, "%s/%s",path.c_str(),name.c_str());
    if(::mkdir(buf, 0666)==0){
        return true;
    }
    
    
    return false;
}

bool File::deleteFile(bool force){
    if(!this->isDir()){
        
        return unlink(path.c_str())==0;
    }
    vector<File> v;
    this->listFiles(v);
    if(v.size()==0){
        return rmdir(path.c_str())==0;
    }
    if(!force){
        
        return false;
        
    }
    for(vector<File>::iterator it = v.begin();it!=v.end();it++){
        if(!(it->deleteFile(true))){
            return false;
        }
    }
    
    
    return rmdir(path.c_str())==0;
}

//
//  io_base.hpp
//  CPP1
//
//  Created by 刘冠宇 on 17/6/17.
//  Copyright © 2017年 刘冠宇. All rights reserved.
//

#ifndef io_base_hpp
#define io_base_hpp
#include "CodeConvert.hpp"
#include <stdio.h>
#include <unistd.h>
class ByteWrite{
public:
    virtual bool write(int byte)=0;
    virtual bool write(const char *buf,off_t len)=0;
    virtual bool write(const char *buf,off_t off,off_t len)=0;
    virtual void flush() =0;

};

class ByteRead{
public:
    virtual bool read(char *t)=0;
    virtual ssize_t read(char *buf,off_t len)=0;
    virtual ssize_t read(char *buf,off_t off,off_t len)=0;
    
};
class FdByteWrite:public virtual ByteWrite{
protected:
    int fd;
public:
    FdByteWrite(int _fd);
    virtual bool write(int byte);
    virtual bool write(const std::string &p);
    virtual bool write(const char *buf,off_t len);
    virtual bool write(const char *buf,off_t off,off_t len);
    virtual bool write(const char *buf,off_t len,CodeConvert &cc);
    virtual bool write(const std::string &p,CodeConvert &cc);
    virtual void flush();
};
class FdByteRead:public virtual ByteRead{
protected:
    int fd;
public:
    FdByteRead(int _fd);
    virtual bool read(char *t);
    virtual ssize_t read(char *buf,off_t len);
    virtual ssize_t read(char *buf,off_t off,off_t len);
    virtual size_t readline(char *buf,size_t len);
    virtual std::string readline();
    virtual std::string readline(const CodeConvert &cc);
};

bool send_fd_un(int un_fd,int tar_fd);
bool recv_fd_un(int un_fd,int *new_fd);

class UnixSocketByteRW:
		public virtual FdByteRead,public virtual FdByteWrite{
public:
	UnixSocketByteRW(int fd);
	~UnixSocketByteRW();
	bool send_fd(int _fd);
	int recv_fd();
protected:
	int un_fd;
};

#endif /* io_base_hpp */

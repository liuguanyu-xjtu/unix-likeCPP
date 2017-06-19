//
//  io_base.cpp
//  CPP1
//
//  Created by 刘冠宇 on 17/6/17.
//  Copyright © 2017年 刘冠宇. All rights reserved.
//

#include "io_base.hpp"
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
FdByteWrite::FdByteWrite(int _fd):fd(_fd){
    
}
bool FdByteWrite::write(int byte){
    char t = (char)byte;
    return ::write(fd,&t,1)==1;
    
}
bool FdByteWrite::write(const std::string &p){
    long l = p.size();
    return ::write(fd, p.c_str(), l)==l;
}
bool FdByteWrite::write(const char *buf,off_t len){
    return ::write(fd, buf, len)==len;
}
bool FdByteWrite::write(const char *buf,off_t off,off_t len){
    return ::write(fd, buf+off, len)==len;
}
void FdByteWrite::flush(){
    fsync(fd);
}
bool FdByteWrite::write(const char *buf,off_t len,CodeConvert &cc){
    size_t rl;
    char *c=cc.convert(buf, len,&rl);
    
    bool q=(::write(fd,c,rl)==rl);
    
    free(c);
    return q;
}
bool FdByteWrite::write(const std::string &p,CodeConvert &cc){
    size_t rl;
    char *c=cc.convert(p.c_str(),p.size(),&rl);
    bool q=(::write(fd,c,rl)==rl);
    free(c);
    return q;
}


FdByteRead::FdByteRead(int _fd):fd(_fd){
    
}
bool FdByteRead::read(char *t){
    char c;
    if(::read(fd, &c, 1)!=1){
        return false;
    };
    *t = c;
    return true;
    
}

ssize_t FdByteRead::read(char *buf,off_t len){
    ssize_t t = ::read(fd, buf, len);
    
    
    return t;
}
ssize_t FdByteRead::read(char *buf,off_t off,off_t len){
    char *ptr = buf+off;
    ssize_t t = ::read(fd, ptr, len);
    return t;
}
std::string FdByteRead::readline(const CodeConvert &cc){
	std::vector<char> v;
	while (true) {
		char c;
		if(!this->read(&c)){
			break;
		}
		if(c=='\n'){
			break;
		}
		v.push_back(c);
	}
	v.push_back('\0');
	unsigned long len;
	char * tmp=cc.convert(&v[0], v.size()-1, &len);
	std::string t(tmp);
	::free(tmp);
	return t;

}
size_t FdByteRead::readline(char *buf,size_t len){
    
    size_t off = 0;
    for(off=0;off<len-1;off++){
        char c;
        if(!this->read(&c)){
            break;
        }
        if(c=='\n'){
            break;
        }
        *(buf+off) = c;
    }
    buf[off]=0;
    return off;
    
}
std::string FdByteRead::readline(){
    std::vector<char> v;
    while (true) {
        char c;
        if(!this->read(&c)){
            break;
        }
        if(c=='\n'){
            break;
        }
        v.push_back(c);
    }
    v.push_back('\0');
    return std::string(&(v[0]));
}

bool send_fd_un(int un_fd,int tar_fd){
#define CONTROLLEN CMSG_LEN(sizeof(int))
	struct iovec  iov[1];
	struct msghdr msg;
	static struct cmsghdr *cmptr = NULL;
	char  buf[2]; /* send_fd()/recv_fd() 2-byte protocol */
	iov[0].iov_base = buf;
	iov[0].iov_len = 2;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	if (tar_fd < 0) {
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	buf[1] = -tar_fd; /* nonzero status means error */
	if (buf[1] == 0)
		buf[1] = 1; /* -256, etc. would screw up protocol */
	}else {
	if (cmptr == NULL && (cmptr = (struct cmsghdr *)malloc(CONTROLLEN)) == NULL)
		return(-1);
		cmptr->cmsg_level = SOL_SOCKET;
		cmptr->cmsg_type = SCM_RIGHTS;
		cmptr->cmsg_len = CONTROLLEN;
		msg.msg_control = cmptr;
		msg.msg_controllen = CONTROLLEN;
		*(int *)CMSG_DATA(cmptr) = tar_fd; /* the fd to pass */


		buf[1] = 0; /* zero status means OK */
	}
	buf[0] = 0; /* null byte flag to recv_fd() */
	if (sendmsg(un_fd, &msg, 0) != 2)
		return(-1);

#undef CONTROLLEN
	return true;
}
bool recv_fd_un(int un_fd,int *_new_fd){

#define CONTROLLEN CMSG_LEN(sizeof(int))
	int  newfd, nr, status;
	char  *ptr;
	static struct cmsghdr *cmptr = NULL;
	char  buf[1024];
	struct iovec iov[1];
	struct msghdr msg;
	status = -1;
	for ( ; ; ) {
	iov[0].iov_base = buf;
	iov[0].iov_len = sizeof(buf);
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	if (cmptr == NULL && (cmptr = (struct cmsghdr *)malloc(CONTROLLEN)) == NULL)
	return(-1);
	msg.msg_control = cmptr;
	msg.msg_controllen = CONTROLLEN;
	if ((nr = recvmsg(un_fd, &msg, 0)) < 0) {
//	err_ret("recvmsg error");
	return(-1);
	}else if (nr == 0) {
//	err_ret("connection closed by server");
	return(-1);

	}
	for (ptr = buf; ptr < &buf[nr]; ) {
	if (*ptr++ == 0) {

	status = *ptr & 0xFF; /* prevent sign extension */
	if (status == 0) {
	if (msg.msg_controllen != CONTROLLEN)
	newfd = *(int *)CMSG_DATA(cmptr);
	}else {
	newfd = -status;
	}
	nr -= 2;
	}
	}
//	if (nr > 0 && (*userfunc)(STDERR_FILENO, buf, nr) != nr)
//	return(-1);
	if (status >= 0) /* final data has arrived */
		*_new_fd=newfd;
	return(newfd); /* descriptor, or -status */
	}
#undef CONTROLLEN
	return false;
}

UnixSocketByteRW::UnixSocketByteRW(int fd):
		un_fd(fd),FdByteWrite(fd),FdByteRead(fd){

}
UnixSocketByteRW::~UnixSocketByteRW(){

}
bool UnixSocketByteRW::send_fd(int _fd){
	return ::send_fd_un(un_fd,_fd);
}
int UnixSocketByteRW::recv_fd(){
	int new_Fd;
	if(::recv_fd_un(un_fd,&new_Fd)){
		return new_Fd;
	}
	return 0;
}




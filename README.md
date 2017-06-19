# unix-likeCPP
this is a bundle of classes writed for unix-like system to make programmer use unix'system calls easily and  effective.Those classes are tested in Mac os and linux.
Note: the prefix is hpp,you can change it to h in cpp file as you like.

CodeConvert.hpp
The convert type class can make Character encoding conversion easy. It use iconv. You don't need to care about the release of iconv Object. It will be done in destructor. Be attention the third function will allocate memory in heap so you need to free it mannually. It's recommend to use the last two function and the std library will allocation and free memory for you.

Note you can use 'man 3 iconv_open' in terminal to look up the coding.
···C++
class CodeConvert{
protected:
    iconv_t type;
public:
    CodeConvert(const char *from,const char *to);
    virtual ~CodeConvert();
    char * convert(const  char *p,const size_t len,size_t *result_len) const;
    bool convert( const char *p,const size_t len,std::string &str) const;
    bool convert(const char *p,const size_t len,char *r,size_t rl) const;
    std::string convert(const std::string &p) const;
    std::string convert(const char *p,const size_t len) const;
};
···
ios_base.hpp
The hpp file contains decorator classes you can use to work with file descripter.Be attention, all file descripter should be close by 'close' funtion in 'unistd.h'. In java language, every  decorator class for stream has a close method. Some stream may has multiple decorator class, if you have closed one and you try to close others you may meet Exception.


This two class are the basic class of byte operation. Because most OS has kernel buffer, it will deal with buffer automatically. The edition don't have buffer. Unlike java, C++ see unicode or other code as bytes while java treat it as utf16 which contains 65536 characters. 

···C++
class ByteWrite{
public:
	virtual bool write(int byte) =0;
	virtual bool write(const std::string &p) =0;
	virtual bool write(const char *buf,off_t len) =0;
	virtual bool write(const char *buf,off_t off,off_t len) =0;
	virtual bool write(const char *buf,off_t len,CodeConvert &cc) =0;
	virtual bool write(const std::string &p,CodeConvert &cc) =0;
	virtual void flush() =0;

};

class ByteRead{
public:
	virtual bool read(char *t)=0;
	virtual ssize_t read(char *buf,off_t len)=0;
	virtual ssize_t read(char *buf,off_t off,off_t len)=0;
	virtual size_t readline(char *buf,size_t len)=0;
	virtual std::string readline()=0;
	virtual std::string readline(const CodeConvert &cc)=0;
    
};
Those class can be used to work with file descripter. Because programmer seldom use tcp'OOB, you can use FdByteRead to manipulate tcp.

'''
class FdByteWrite:public virtual ByteWrite{
protected:
    int fd;
public:
    FdByteWrite(int _fd); 
};

class FdByteRead:public virtual ByteRead{
protected:
    int fd;
public:
    FdByteRead(int _fd);
};
class FdByteRW:public virtual FdByteRead,public virtual FdByteWrite{
	FdByteRW(int _fd):FdByteWrite(_fd),FdByteRead(_fd){}
};
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

```

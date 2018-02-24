#include "NetBuffer.h"

const int ZK_ImageServer::net::NetBuffer::defaultBufferSize = 1024;
const int ZK_ImageServer::net::NetBuffer::defaultPrependSize = 8;


ZK_ImageServer::net::NetBuffer::NetBuffer(int buffersize):
prependSize_(defaultPrependSize),
bufferSize_(buffersize),
buffer_(prependSize_ + bufferSize_),
readIndex_(prependSize_),
writeIndex_(prependSize_)
{

}

ZK_ImageServer::net::NetBuffer::~NetBuffer()
{
    //dtor
}

size_t ZK_ImageServer::net::NetBuffer::readFd(int fd)
{
    char extraBuffer[65536];
    memset(extraBuffer,0,sizeof(extraBuffer));
    struct iovec iov[2];
    iov[0].iov_base = Begin() + writeIndex_;
    int writeablebyte = writeableByte();
    iov[0].iov_len = writeablebyte;
    iov[1].iov_base = extraBuffer;
    iov[1].iov_len = sizeof(extraBuffer);
    int len = readv(fd,iov,2);
    if(len < 0)
    {
        LOG_ERROR << "read error in NetBuffer";
    }
    else if(len <= writeablebyte)
    {
        writeIndex_ += len;
    }
    else
    {
        writeIndex_ = buffer_.size();
    }

    return len;
}

void ZK_ImageServer::net::NetBuffer::append(void* data,int len)
{
    if(writeableByte() < len)
    {
        getSpace(len);
    }
    assert(writeableByte() >= len);
    memcpy(writeBegin(),data,len);
    writeIndex_ += len;
}

void ZK_ImageServer::net::NetBuffer::getSpace(int len)
{
    int availByte = writeableByte() + headAvailByte();
    if(availByte < len)
    {
        buffer_.resize(writeIndex_ + len);
    }
    else
    {
        int readablebyte = readableByte();
        std::copy(Begin()+readIndex_,Begin()+writeIndex_,Begin()+prependSize_);
        readIndex_ = prependSize_;
        writeIndex_ = readIndex_ + readablebyte;
    }
}

void ZK_ImageServer::net::NetBuffer::retrieve(int len)
{
    if(len <= readableByte())
    {
        readIndex_ += len;
    }
    else
    {
        retrieveAll();
        LOG_WARNING << "can not retrieve "<< len <<" byte,just retrieve availiable byte";
    }

}

void ZK_ImageServer::net::NetBuffer::retrieveAll()
{
    readIndex_ = prependSize_;
    writeIndex_ = prependSize_;
}

std::string ZK_ImageServer::net::NetBuffer::retrieveAsString(int len)
{
    assert(len <= readableByte());
    std::string temp(readBegin(),len);
    retrieve(len);
    return temp;
}

std::string ZK_ImageServer::net::NetBuffer::getAsString(int len)
{
    assert(len <= readableByte());
    std::string temp(readBegin(),len);
    return temp;
}

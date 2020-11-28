#pragma once
#include "vector"
#include "asio.hpp"
using asio::ip::udp;
class IReciever
{
public:
    virtual int AsyncReceive(std::vector<unsigned char> &) = 0;
    virtual int Receive(std::vector<unsigned char> &) = 0;
    virtual ~IReciever(){};
};
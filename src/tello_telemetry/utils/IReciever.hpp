#pragma once
#include "vector"
#include "asio.hpp"
using asio::ip::udp;
class IReciever
{
public:
    virtual int Recieve(std::vector<unsigned char> &data) = 0;
    virtual ~IReciever(){};
};
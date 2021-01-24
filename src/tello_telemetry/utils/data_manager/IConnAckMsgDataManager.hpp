#pragma once
#include <vector>
class IConnAckMsgDataManager
{
public:
    virtual void SetConnReqAck() = 0;
    virtual ~IConnAckMsgDataManager(){};
};

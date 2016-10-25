#ifndef _MDP_WORKER_H_
#define _MDP_WORKER_H_

#include "mdp_define.h"

namspace mdp {

class WrkApi
{
public:
    WrkApi();
    virtual ~WrkApi();
    int Init(const std::string& broker, const std::string& service);
    int RecvMsg(std::string& msg, std::string& from);
    int SendMsg(std::string& msg, std::string& to);
    int SetSockOpt(int option, const void *optval, size_t optvallen);
    int GetSockOpt(int option, void *optval, size_t *optvallen);

private:
    int  RealSend(const std::string& cmd, std::string& option, std::string& msg);
    int  Connect();
    void Close();
    int  ProcessCmd();

private:
    void*           m_ctx;
    void*           m_socket;
    std::string     m_broker;
    std::string     m_service;
    int             m_heartbeat; //  Heartbeat delay, msecs
    int             m_reconnect; //  Reconnect delay, msecs
    size_t          m_liveness;
};

}

#endif//_MDP_WORKER_H_

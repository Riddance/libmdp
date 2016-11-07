#ifndef _MDP_WRK_H_
#define _MDP_WRK_H_

#include "mdp_define.h"
#include <string>

namspace mdp {

class WrkApi
{
public:
    WrkApi();
    virtual ~WrkApi();

    int Init(const std::string& broker, const std::string& service);
    int Recv(std::string& msg, std::string& identity);
    int Send(std::string& msg, std::string& identity);
    int SetSockOpt(int option, const void *optval, size_t optvallen);
    int GetSockOpt(int option, void *optval, size_t *optvallen);

private:
    int  Connect();
    void Close();
    int  ProcessRecvCmd(mdp::MdpMsg& mdp_message, std::string& message, std::string& from);
    int  RealSend(const std::string& cmd, const std::string& opt, mdp::MdpMsg& mdp_message);
    void DoHeartbeat();
    
private:
    void*           m_ctx;
    void*           m_socket;
    std::string     m_broker;
    std::string     m_service;
    int             m_heartbeat_at;
};

}

#endif//_MDP_WRK_H_

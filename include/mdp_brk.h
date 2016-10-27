#ifndef MDP_BRK_H
#define MDP_BRK_H

namespace mdp {

class BrkApi
{
public:
    BrkApi(){}
    virtual ~BrkApi(){}

    int  Init(const std::string& endpoint);
    void StartBrokering();
    void Stop();


private:
    int   Bind();
    int   Close();
    void  ProcessClientMessage(mdp::MdpMessage& mdp_message, std::string& sender);
    void  ProcessWorkerMessage(mdp::MdpMessage& mdp_message, std::string& sender);
    void  DoHeartbeat();

private:
    void*           m_ctx;
    void*           m_socket;
    int             m_heartbeat_at;
    std::string     m_endpoint;
    bool            m_quit;
};

} // mdp

#endif // MDP_BRK_H

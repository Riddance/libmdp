#ifndef MDP_BRK_H
#define MDP_BRK_H

namespace mdp {

class BrkApi
{
public:
    BrkApi(){}
    virtual ~BrkApi(){}
    int Init(const std::string& endpoint);


private:
    int Bind();
    int Close();

private:
    void*           m_ctx;
    void*           m_socket;
    int             m_heartbeat_at;
    std::string     m_endpoint;
};

} // mdp

#endif // MDP_BRK_H

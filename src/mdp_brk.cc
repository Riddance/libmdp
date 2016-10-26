#include "mdp_brk.h"

namespace mdp {

BrkApi::BrkApi()
{

}

BrkApi::~BrkApi()
{
    Close();
}

int BrkApi::Init(const std::string& endpoint)
{
    m_ctx = zmq_ctx_new();
    if (m_ctx == NULL)
        return -1;
    
    return Bind();
}

int BrkApi::Bind()
{
    MDP_ASSERT (m_socket == NULL);
    m_socket = zmq_socket (m_ctx, ZMQ_ROUTER);
    if (socket_ == NULL)
        return -1;

    int rc = zmq_bind(m_socket, m_endpoint);
    if (rc != 0)
        return rc;

    m_heartbeat_at = mdp::mdp_time () + HEARTBEAT_INTERVAL;

    return 0;
}

} // mdp

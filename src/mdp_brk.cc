#include "mdp_brk.h"

namespace mdp {

BrkApi::BrkApi()
{
    m_quit      = false;
    m_ctx       = NULL;
    m_socket    = NULL;
    m_heartbeat = 2500;
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

void BrkApi::StartBrokering()
{
    while (!m_quit)
    {
        int now = mdp::mdp_time ();
        
        zmq_pollitem_t items [] = {m_socket, 0, ZMQ_POLLIN, 0};
        int time_out = heartbeat_at - now;
        rc = zmq_poll (items, 1, 
                      m_heartbeat * ZMQ_POLL_MSEC);

        if (items [0].revents & ZMQ_POLLIN) {
            mdp::MdpMessage mdp_message;
            rc = mdp_message.Recv (m_socket);
            if (rc != 0)
                return rc;

            std::string sender = mdp_message.PopFront ();
            mdp_message.PopFront();
            std::string header = mdp_message.PopFront ();

            if (header == std::string (MDPC_CLIENT))
            {
                ProcessClientMessage (mdp_message, sender);
            }
            else if (header == std::string (MDPW_WORKER))
            {
                ProcessWorkerMessage (mdp_message, sender);
            }
        }

        now = mdp::mdp_time ();
        if (now >= heartbeat_at) {
            DoHeartbeat ();
        }
    }
}

void BrkApi::Stop()
{
    m_quit = true;
}

void BrkApi::DoHeartbeat()
{

}

void BrkApi::ProcessClientMessage(mdp::MdpMessage& mdp_message, std::string& sender)
{

}

void BrkApi::ProcessWorkerMessage(mdp::MdpMessage& mdp_message, std::string& sender)
{

}

} // mdp

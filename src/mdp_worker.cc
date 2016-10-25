#include "mdp_worker.h"
#include "mdp_msg.h"

namespace mdp {

WrkApi::WrkApi()
{
    m_ctx       = NULL;
    m_socket    = NULL;
    m_heartbeat = 2500;
    m_reconnect = 2500;//  msecs
}

WrkApi::~WrkApi()
{
    Close();
}

int WrkApi::Init(const std::string& broker, const std::string& service)
{
    m_broker    = broker;
    m_service   = service;

    m_ctx = zmq_ctx_new();
    if (m_ctx == NULL)
        return -1;

    return Connect ();
}

int WrkApi::RecvMsg(std::string& msg, std::string& from)
{
    int rc = 0;

    while (true)
    {
        zmq_pollitem_t items [] = {m_socket, 0, ZMQ_POLLIN, 0};
        rc = zmq_poll (items, 1, 
                      m_heartbeat * ZMQ_POLL_MSEC);
        if (rc == -1)
            break;

        if (items[0].revents & ZMQ_POLLIN)
        {
            mdp::MdpMsg mdp_msg;
            rc = mdp_msg.Recv (m_socket);
            if (rc != 0)
                break;

            ZMQ_ASSERT (mdp_msg.size() >= 3);
            std::string empty = mdp_msg.PopFront ();
            std::string cmd   = mdp_msg.PopFront ();
            ProcessCmd();
            //if (command == std::string(MDPW_REQUEST))

        }
    }

    return 0;
}

int WrkApi::SendMsg (std::string& msg, std::string& to)
{
    return RealSend();
}

int WrkApi::SetSockOpt (int option, const void *optval, size_t optvallen)
{
    ZMQ_ASSERT (m_socket);
    return zmq_setsockopt (m_socket, option, optval, optvallen);
}

int WrkApi::GetSockOpt (int option, void *optval, size_t *optvallen)
{
    ZMQ_ASSERT (m_socket);
    return zmq_getsockopt (m_socket, option, optval, optvallen);
}

int WrkApi::Connect ()
{
    int rc = 0;
    if (m_socket != NULL)
        rc = zmq_close (m_socket);

    ZMQ_ASSERT (rc == 0);
    m_socket = zmq_socket (m_ctx, ZMQ_DEALER);
    if (socket_ == NULL)
        return -1;

    rc = zmq_connect (m_socket, m_broker.c_str());
    if (rc != 0)
        return rc;

    m_liveness = HEARTBEAT_LIVENESS;
    m_heartbeat = s_clock () + m_heartbeat;

    return RealSend ((char*)MDPW_READY, m_service, "");
}

int WrkApi::RealSend (const std::string& cmd, const std::string& dst, const std::string& data)
{
    mdp::MdpMsg mdp_msg;
    if (!dst.empty()) {
        mdp_msg.PushFront (dst);
        mdp_msg.PushFront ("");
    }

    mdp_msg.PushFront (cmd);
    mdp_msg.PushFront (MDPW_WORKER);
    mdp_msg.PushFront ("");

    return mdp_msg.Send (m_socket);
}

void WrkApi::Close ()
{
    int rc = RealSend (MDPW_DISCONNECT, "", "");
    ZMQ_ASSERT (rc == 0);

    if (m_socket != 0)
    {
        rc = zmq_close (m_socket);
        ZMQ_ASSERT (rc == 0);
        m_socket = NULL;
    }

    if (m_ctx != 0)
    {
        rc = zmq_ctx_destroy (m_ctx);
        ZMQ_ASSERT (rc == 0);
        m_ctx = NULL;
    }

    return;
}

} // mdp


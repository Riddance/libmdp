#include "mdp_wrk.h"
#include "mdp_message.h"

namespace mdp {

WrkApi::WrkApi()
{
    m_ctx           = NULL;
    m_socket        = NULL;
    m_heartbeat     = 2500;
    m_heartbeat_at  = 0;
}

WrkApi::~WrkApi()
{
    Close();
}

int WrkApi::Init(const std::string& broker, const std::string& service)
{
    m_broker    = broker;
    m_service   = service;

    m_ctx = zmq_ctx_new ();
    if (m_ctx == NULL)
        return -1;

    return Connect ();
}

int WrkApi::RecvMessage(std::string& message, std::string& addr)
{
    int rc = 0;
    MDP_ASSERT (message.empty());
    MDP_ASSERT (addr.empty());

    zmq_pollitem_t items [] = {m_socket, 0, ZMQ_POLLIN, 0};
    rc = zmq_poll (items, 1, 0);
    if (rc == -1)
        return rc;

    if (items[0].revents & ZMQ_POLLIN)
    {
        mdp::MdpMsg mdp_message;
        rc = mdp_message.Recv (m_socket);
        if (rc != 0)
            return rc;

        rc = ProcessRecvCmd (mdp_message, message, addr);
        if (rc != 0)
            return rc;
    }

    DoHeartbeat ();

    return 0;
}

int WrkApi::SendMessage (std::string& msg, std::string& to)
{
    MDP_ASSERT (!to.empty());

    mdp::MdpMsg mdp_message;
    mdp_message.PushFront("");
    mdp_message.PushFront(to);

    return RealSend(MDPW_REPORT, "", msg);
}

int WrkApi::SetSockOpt (int option, const void *optval, size_t optvallen)
{
    MDP_ASSERT (m_socket);
    return zmq_setsockopt (m_socket, option, optval, optvallen);
}

int WrkApi::GetSockOpt (int option, void *optval, size_t *optvallen)
{
    MDP_ASSERT (m_socket);
    return zmq_getsockopt (m_socket, option, optval, optvallen);
}

int WrkApi::Connect ()
{
    int rc = 0;
    if (m_socket != NULL)
        rc = zmq_close (m_socket);

    MDP_ASSERT (rc == 0);
    m_socket = zmq_socket (m_ctx, ZMQ_DEALER);
    if (socket_ == NULL)
        return -1;

    rc = zmq_connect (m_socket, m_broker.c_str());
    if (rc != 0)
        return rc;

    m_heartbeat = mdp::mdp_time() + m_heartbeat;

    mdp::MdpMsg mdp_message;
    mdp_message.PushFront ();
    return RealSend ((char*)MDPW_READY, m_service, mdp_message);
}

void WrkApi::Close ()
{
    mdp::MdpMsg mdp_message;
    int rc = RealSend (MDPW_DISCONNECT, "", mdp_message);
    MDP_ASSERT (rc == 0);

    if (m_socket != 0)
    {
        rc = zmq_close (m_socket);
        MDP_ASSERT (rc == 0);
        m_socket = NULL;
    }

    if (m_ctx != 0)
    {
        rc = zmq_ctx_destroy (m_ctx);
        MDP_ASSERT (rc == 0);
        m_ctx = NULL;
    }

    return;
}

int WrkApi::ProcessRecvCmd (mdp::MdpMsg& mdp_message, std::string& message, std::string& from)
{
    int rc = 0;
    MDP_ASSERT (mdp_message.size() >= 3);
    std::string empty   = mdp_message.PopFront ();
    std::string header  = mdp_message.PopFront ();
    MDP_ASSERT (header == std::string(MDPW_WORKER));
    std::string cmd = mdp_message.PopFront ();

    if (cmd == std::string (MDPW_HEARTBEAT)) {;}
    else if (cmd == std::string (MDPW_REQUEST))
    {
        MDP_ASSERT (mdp_message.size() == 3);
        from    = mdp_message.PopFront();
        empty   = mdp_message.PopFront();
        message = mdp_message.PopFront();
    }
    else if (cmd == std::string (MDPW_DISCONNECT))
    {
        rc = Connect();
        if (rc != 0)
            return rc;
    }

    return 0;
}

void WrkApi::DoHeartbeat()
{
    if (mdp::mdp_time() > m_heartbeat_at)
    {
        mdp::MdpMsg mdp_message;
        RealSend(MDPW_HEARTBEAT, "", mdp_message);
        m_heartbeat_at = mdp::mdp_time() + m_heartbeat;
    }

    return;
}


int WrkApi::RealSend (const std::string& cmd, const std::string& opt, mdp::MdpMsg& mdp_message)
{
    if (!opt.empty())
        mdp_message.PushFront(opt);

    mdp_message.PushFront (cmd);
    mdp_message.PushFront (MDPW_WORKER);
    mdp_message.PushFront ("");

    return mdp_message.Send (m_socket);
}


} // mdp

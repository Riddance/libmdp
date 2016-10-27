#include "mdp_cli.h"

namespace mdp {

SyncCliApi::SyncCliApi()
{
    m_ctx       = NULL;
    m_socket    = NULL;
    m_timeout   = 0;
}

SyncCliApi::~SyncCliApi()
{
    Close();
}

int SyncCliApi::Init(const std::string& broker)
{
    m_broker = broker;
    m_ctx = zmq_ctx_new();
    if (m_ctx == NULL)
        return -1;

    return Connect();
}

int SyncCliApi::Request(const std::string& service, std::string& req, std::string& rep)
{
    int rc = 0;

    mdp::MdpMessage req_message;
    req_message.PushFront(req);
    req_message.PushFront(service);
    req_message.PushFront(MDPC_CLIENT);
    rc = req_message.Send(m_socket);
    if (rc != 0)
        return rc;

    zmq_pollitem_t items [] = {m_socket, 0, ZMQ_POLLIN, 0};
    rc = zmq_poll (items, 1, m_timeout);
    if (rc == -1)
        return rc;

    if (items[0].revents & ZMQ_POLLIN)
    {
        mdp::MdpMessage rep_message;
        rc = rep_message.Recv (m_socket);
        if (rc != 0)
            return rc;

        std::string header  = rep_message.PopFront ();
        std::string service = rep_message.PopFront ();

        rep = rep_message.PopFront();
    }

    return 0;
}

int SyncCliApi::Connect()
{
    int rc = 0;
    if (m_socket != NULL)
        rc = zmq_close (m_socket);

    MDP_ASSERT (rc == 0);
    m_socket = zmq_socket (m_ctx, ZMQ_REQ);
    if (socket_ == NULL)
        return -1;

    return zmq_connect (m_socket, m_broker.c_str());
}

void SyncCliApi::Close()
{
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


AsyncCliApi::AsyncCliApi()
{

}

AsyncCliApi::~AsyncCliApi()
{

}

int AsyncCliApi::Init(const std::string& broker)
{
    m_broker = broker;
    m_ctx = zmq_ctx_new();
    if (m_ctx == NULL)
        return -1;

    return Connect();
}

int AsyncCliApi::Connect()
{
    int rc = 0;
    if (m_socket != NULL)
        rc = zmq_close (m_socket);

    MDP_ASSERT (rc == 0);
    m_socket = zmq_socket (m_ctx, ZMQ_DEALER);
    if (socket_ == NULL)
        return -1;

    return zmq_connect (m_socket, m_broker.c_str());
}

void AsyncCliApi::Close()
{
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



} // mdp
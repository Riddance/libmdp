#include "mdp_cli.h"

namespace mdp {


SyncCliApi::SyncCliApi ()
{
    m_ctx       = NULL;
    m_socket    = NULL;
    m_timeout   = 2500;
}

SyncCliApi::~SyncCliApi ()
{
    Close();
}

int SyncCliApi::Init (const std::string& broker)
{
    m_broker = broker;
    m_ctx = zmq_ctx_new();
    if (m_ctx == NULL)
        return -1;

    return 0;
}

int SyncCliApi::Request (const std::string& service, std::string& req, std::string& rep)
{
    int rc = Connect ();
    if (rc != 0)
        return rc;

    mdp::MdpMsg req_message;
    req_message.PushFront (req);
    req_message.PushFront (service);
    req_message.PushFront (MDPC_CLIENT);
    rc = req_message.Send (m_socket);
    if (rc != 0)
        return rc;

    zmq_pollitem_t items [] = {m_socket, 0, ZMQ_POLLIN, 0};
    rc = zmq_poll (items, 1, m_timeout);
    if (rc == -1)
        return rc;

    if (items[0].revents & ZMQ_POLLIN)
    {
        mdp::MdpMsg rep_message;
        rc = rep_message.Recv (m_socket);
        if (rc != 0)
            return rc;

        std::string header  = rep_message.PopFront ();
        std::string service = rep_message.PopFront ();

        rep = rep_message.PopFront ();
    }

    return 0;
}

void SyncCliApi::SetTimeOut(int timeout)
{
    m_timeout = timeout;
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

    int linger = 0;
    rc = zmq_setsockopt (m_socket, ZMQ_LINGER, &linger, sizeof (linger));
    if (rc != 0)
        return rc;

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
    m_ctx       = NULL;
    m_socket    = NULL;
    m_timeout   = 0;
}

AsyncCliApi::~AsyncCliApi()
{
    Close();
}

int AsyncCliApi::Init(const std::string& broker)
{
    m_broker = broker;
    m_ctx = zmq_ctx_new();
    if (m_ctx == NULL)
        return -1;

    return 0;
}

void AsyncCliApi::SetTimeOut(int timeout)
{
    m_timeout = timeout;
}

int AsyncCliApi::Send(const std::string& service, std::string& message)
{
    mdp::MdpMsg req_message;

    req_message.PushFront (message);
    req_message.PushFront (service);
    req_message.PushFront (MDPC_CLIENT);
    req_message.PushFront ("");

    int rc = req_message.Send(m_socket);
    if (rc != 0)
        return rc;

    return 0;
}

int AsyncCliApi::Recv(std::string& service, std::string& message)
{
    zmq_pollitem_t items [] = {m_socket, 0, ZMQ_POLLIN, 0};
    int rc = zmq_poll (items, 1, m_timeout);
    if (rc == -1)
        return rc;

    if (items[0].revents & ZMQ_POLLIN)
    {
        mdp::MdpMsg rep_message;
        rc = rep_message.Recv (m_socket);
        if (rc != 0)
            return rc;

        std::string header;
        std::string service;

        header  = rep_message.PopFront ();
        service = rep_message.PopFront ();
        message = rep_message.PopFront ();
    }

    return 0;
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

    int linger = 0;
    rc = zmq_setsockopt (m_socket, ZMQ_LINGER, &linger, sizeof (linger));
    if (rc != 0)
        return rc;

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
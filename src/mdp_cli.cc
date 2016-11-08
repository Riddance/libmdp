#include "mdp_cli.h"
#include "mdp_define.h"

namespace mdp {

SyncCliApi::SyncCliApi()
{
    m_ctx       = NULL;
    m_socket    = NULL;
    m_timeout   = 2500;
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

    return 0;
}

int SyncCliApi::SetSockOpt(int option, const void *optval, size_t optvallen)
{
    MDP_ASSERT(m_socket);
    return zmq_setsockopt(m_socket, option, optval, optvallen);
}

int SyncCliApi::GetSockOpt(int option, void *optval, size_t *optvallen)
{
    MDP_ASSERT(m_socket);
    return zmq_getsockopt(m_socket, option, optval, optvallen);
}

int SyncCliApi::Request(const std::string& service,
                        const std::string& route_data,
                        const std::string& req, std::string& rep)
{
    int rc = Connect();
    if (rc != 0)
        return rc;

    mdp::MdpMsg req_message;
    req_message.PushFront(req);
    req_message.PushFront(route_data);
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
        mdp::MdpMsg rep_message;
        rc = rep_message.Recv(m_socket);
        if (rc != 0)
            return rc;

        std::string header  = rep_message.PopFront();
        std::string service = rep_message.PopFront();

        rep = rep_message.PopFront();
    }

    return (int) rep.size();
}

void SyncCliApi::SetTimeOut (int timeout)
{
    m_timeout = timeout;
}

int SyncCliApi::Connect ()
{
    int rc = 0;
    if (m_socket != NULL)
        rc = zmq_close(m_socket);

    MDP_ASSERT(rc == 0);
    m_socket = zmq_socket(m_ctx, ZMQ_REQ);
    if (m_socket == NULL)
        return -1;

    int linger = 0;
    rc = zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof (linger));
    if (rc != 0)
        return rc;

    return zmq_connect(m_socket, m_broker.c_str());
}

void SyncCliApi::Close()
{
    int rc = 0;
    if (m_socket != 0)
    {
        rc = zmq_close(m_socket);
        MDP_ASSERT (rc == 0);
        m_socket = NULL;
    }

    if (m_ctx != 0)
    {
        rc = zmq_ctx_destroy(m_ctx);
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

int AsyncCliApi::Init (const std::string& broker)
{
    m_broker = broker;
    m_ctx = zmq_ctx_new();
    if (m_ctx == NULL)
        return -1;

    return 0;
}

int AsyncCliApi::SetSockOpt (int option, const void *optval, size_t optvallen)
{
    MDP_ASSERT (m_socket);
    return zmq_setsockopt(m_socket, option, optval, optvallen);
}

int AsyncCliApi::GetSockOpt (int option, void *optval, size_t *optvallen)
{
    MDP_ASSERT (m_socket);
    return zmq_getsockopt(m_socket, option, optval, optvallen);
}

void AsyncCliApi::SetTimeOut (int timeout)
{
    m_timeout = timeout;
}

int AsyncCliApi::Send(const std::string& service, const std::string& route_data, std::string& message)
{
    mdp::MdpMsg req_message;

    req_message.PushFront(message);
    req_message.PushFront(route_data);
    req_message.PushFront(service);
    req_message.PushFront(MDPC_CLIENT);
    req_message.PushFront("");

    int rc = req_message.Send(m_socket);
    if (rc != 0)
        return rc;

    return 0;
}

//return recv data size
int AsyncCliApi::Recv(std::string& service, std::string& message)
{
    MDP_ASSERT(service.empty());
    MDP_ASSERT(message.empty());
    
    zmq_pollitem_t items[] = {m_socket, 0, ZMQ_POLLIN, 0};
    int rc = zmq_poll(items, 1, m_timeout);
    if (rc == -1)
        return rc;

    if (items[0].revents & ZMQ_POLLIN)
    {
        mdp::MdpMsg rep_message;
        rc = rep_message.Recv(m_socket);
        if (rc != 0)
            return rc;

        std::string header;
        std::string service;

        header  = rep_message.PopFront();
        service = rep_message.PopFront();
        message = rep_message.PopFront();
    }

    return (int) message.size ();
}

int AsyncCliApi::Connect()
{
    int rc = 0;
    if (m_socket != NULL)
        rc = zmq_close(m_socket);

    MDP_ASSERT(rc == 0);
    m_socket = zmq_socket(m_ctx, ZMQ_DEALER);
    if (m_socket == NULL)
        return -1;

    int linger = 0;
    rc = zmq_setsockopt(m_socket, ZMQ_LINGER, &linger, sizeof (linger));
    if (rc != 0)
        return rc;

    return zmq_connect(m_socket, m_broker.c_str());
}

void AsyncCliApi::Close()
{
    int rc = 0;
    if (m_socket != 0)
    {
        rc = zmq_close(m_socket);
        MDP_ASSERT(rc == 0);
        m_socket = NULL;
    }

    if (m_ctx != 0)
    {
        rc = zmq_ctx_destroy(m_ctx);
        MDP_ASSERT(rc == 0);
        m_ctx = NULL;
    }

    return;
}

} // mdp

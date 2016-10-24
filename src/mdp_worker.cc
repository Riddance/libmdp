#include "mdp_worker.h"


WorkerApi::~WorkerApi()
{
    Close();
}

bool WorkerApi::Init(const std::string& broker, const std::string& service)
{
    m_broker    = broker;
    m_service   = service;

    m_ctx = zmq_ctx_new();
    if (m_ctx == NULL)
        return false;

    if (!ConnectToBroker()) {
        return false;
    }

    return true;
}

bool WorkerApi::Recv(std::string& msg, std::string& from)
{


}

bool WorkerApi::Send(std::string& msg, std::string& to)
{

}

bool WorkerApi::ConnectToBroker()
{
    int ret = 0;
    if (m_socket != 0)
    {
        ret = zmq_close(m_socket);
        ZMQ_ASSERT(ret == 0);
        m_socket = 0;
    }

    m_socket = zmq_socket(m_zmq_ctx, ZMQ_DEALER);
    if (m_socket == 0)
        return false;

    ret = zmq_connect(m_socket, m_broker.c_str());
    if (ret != 0)
        return false;

    if (SendToBroker())
        return false;

    return true;
}

bool WorkerApi::SendToBroker(const std::string& command, std::string& option, std::string& msg)
{
    std::list<zmq::zmq_msg_t> msg_list;

    if (option.size() != 0) {
        if(!PushMsg(msg_list, option))
            return false;
    }

    if (!PushMsg(msg_list, command))
        return false;

    if (!PushMsg(msg_list, MDPW_WORKER))
        return false;

    if (!PushMsg(msg_list, ""))
        return false;

    return SendMsgList(msg_list);
}

bool WorkerApi::PushMsg(std::list<zmq::zmq_msg_t>& msg_list, const std::string& msg)
{
    msg_list.push_front(zmq::zmq_msg_t);
    zmq::zmq_msg_t& msg_t = msg_list.front();

    if (msg.size() != 0)
        return zmq_msg_init_data(&msg_t, msg.c_str(), msg.size(), NULL, NULL) == 0;
    else 
        return zmq_msg_init(&msg_t) == 0;
}

void WorkerApi::Close()
{
    int ret = 0;
    SendToBroker();

    if (m_socket != 0)
    {
        ret = zmq_close(m_socket);
        ZMQ_ASSERT(ret == 0);
        m_socket = 0;
    }

    if (m_ctx != 0)
    {
        ret = zmq_ctx_destroy(m_ctx);
        ZMQ_ASSERT(ret == 0);
        m_ctx = 0;
    }

    return;
}

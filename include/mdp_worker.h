#ifndef _MDP_WORKER_H_
#define _MDP_WORKER_H_

namspace mdp {

class WorkerApi
{
public:
    WorkerApi();
    virtual ~WorkerApi();

    bool Init(const std::string& broker, const std::string& service);
    bool Recv(std::string& msg, std::string& from);
    bool Send(std::string& msg, std::string& to);
    
private:
    bool SendToBroker(const std::string& command, std::string& option, std::string& msg);
    bool ConnectToBroker();
    void Close();
    bool PushMsg(std::list<zmq::zmq_msg_t>& msg_list);

private:
    void*           m_ctx;
    void*           m_socket;
    std::string     m_broker;
    std::string     m_service;
    int             m_heartbeat; //  Heartbeat delay, msecs
    int             m_reconnect; //  Reconnect delay, msecs
    size_t          m_liveness;
};

}

#endif//_MDP_WORKER_H_

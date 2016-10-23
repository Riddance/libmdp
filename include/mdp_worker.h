#ifndef _MDP_WORKER_H_
#define _MDP_WORKER_H_

namspace mdp {

class WorkerApi
{
public:
    WorkerApi();
    virtual WorkerApi();

    bool Init(const std::string& broker, const std::string& service);
    bool Recv(zmq_msg_t& msg,   zmq_msg_t& from);
    bool Send(zmq_msg_t& to,    zmq_msg_t& msg);
    
private:
    bool ConnectToBroker();

private:
    void*           m_zmq_ctx;
    void*           m_zmq_socket;
    std::string     m_broker;
    std::string     m_service;
    int             m_heartbeat; //  Heartbeat delay, msecs
    int             m_reconnect; //  Reconnect delay, msecs
};

}

#endif//_MDP_WORKER_H_

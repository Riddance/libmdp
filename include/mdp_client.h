#ifndef _MDP_CLIENT_H_
#define _MDP_CLIENT_H_

namspace mdp {

class ClientApi
{
public:
    ClientApi();
    virtual ~ClientApi();

private:
    void*     m_zmq_ctx;
    void*     m_zmq_socket;
};

class SyncClientApi
{
public:
    ClientApi();
    virtual ~ClientApi();

    bool Init(const std::string& broker);
    bool Send(const std::string& msg, const std::string& service);
    bool Recv(std::string& msg, std::string& service);
    bool SetSockOpt();
    bool GetSockOpt();

private:
    void*           m_zmq_ctx;
    void*           m_zmq_socket;
};


class AsyncClientApi
{
public:
    ClientApi();
    virtual ~ClientApi();

    bool Init(const std::string& broker);
    bool Send(const std::string& msg, const std::string& service);
    bool Recv(std::string& msg, std::string& service);
    bool SetSockOpt();
    bool GetSockOpt();

private:
    void*           m_zmq_ctx;
    void*           m_zmq_socket;
};

}

#endif//_MDP_CLIENT_H_

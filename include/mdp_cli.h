#ifndef _MDP_CLI_H_
#define _MDP_CLI_H_

namspace mdp {

class SyncCliApi
{
public:
    SyncCliApi();
    virtual ~SyncCliApi();
    int     Init (std::string& broker);
    int     Request (const std::string& service, std::string& req, std::string& rep);
    void    SetTimeOut (int timeout);

private:
    int     Connect ();

private:
    void*       m_zmq_ctx;
    void*       m_zmq_socket;
    std::string m_broker;
    int         m_timeout;
};


class AsyncCliApi
{
public:
    AsyncCliApi();
    virtual ~AsyncCliApi();

    int     Init(std::string& broker);
    int     Send(const std::string& service, std::string& message);
    int     Recv(std::string& service, std::string& message);
    void    SetTimeOut(int timeout);

private:
    int Connect();

private:
    void*       m_zmq_ctx;
    void*       m_zmq_socket;
    std::string m_broker;
    int         m_timeout;
};

}

#endif//_MDP_CLI_H_

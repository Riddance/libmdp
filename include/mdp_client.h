#ifndef _MDP_CLIENT_H_
#define _MDP_CLIENT_H_

namspace mdp {
    
class cliapi
{
public:
    cliapi();
    virtual ~cliapi();

private:
    void*     m_zmq_ctx;
    void*     m_zmq_socket;
};

}

#endif//_MDP_CLIENT_H_

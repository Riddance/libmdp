#ifndef _MDP_BROKER_H_
#define _MDP_BROKER_H_

namspace mdp {

class BrkApi
{
public:
    BrkApi();
    virtual BrkApi();

    int init();

private:
    void*           m_ctx;
    void*           m_socket;
    std::string     m_broker;
};

}

#endif//_MDP_BROKER_H_

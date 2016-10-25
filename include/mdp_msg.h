#ifndef MDP_MSG_H
#define MDP_MSG_H

#include <list>
#include <zmq.h>
#include "mdp_define.h"

namespace mdp {

class MdpMsg
{
public:
    MdpMsg(){}
    virtual ~MdpMsg()
    {
        Clear();
    }

    void PushFront(const std::string& data)
    {
        int rc = 0;
        m_parts.push_front(zmq_msg_t());
        zmq_msg_t& msg_t = msg_list.front();
        if (data.empty())
            rc = zmq_msg_init(&msg_t);
        else
            rc = zmq_msg_init_data(&msg_t, data.c_str(), 
                                   data.size(), 0, 0);
        ZMQ_ASSERT(rc == 0);

        return;
    }

    void PushBack(const std::string& data)
    {
        int rc = 0;
        m_parts.push_back(zmq_msg_t());
        zmq_msg_t& msg_t = msg_list.front();
        if (data.empty())
            rc = zmq_msg_init(&msg_t);
        else
            rc = zmq_msg_init_data(&msg_t, data.c_str(), 
                                   data.size(), 0, 0);
        ZMQ_ASSERT(rc == 0);
        
        return;
    }

    std::string PopFront ()
    {
        zmq_msg_t& msg_t = msg_list.front();
        std::string data((char*)zmq_msg_data(&msg_t));
        zmq_msg_close(&msg_t);
        msg_list.pop_front();

        return data;
    }

    std::string PopBack ()
    {
        zmq_msg_t& msg_t = msg_list.back();
        std::string data((char*)zmq_msg_data(&msg_t));
        zmq_msg_close(&msg_t);
        msg_list.pop_back();

        return data;
    }

    int Send (void* socket)
    {
        ZMQ_ASSERT(socket);
        int rc = 0;

        while(!m_parts.empty()) {
            zmq_msg_t& msg_t = msg_list.front();
            rc = zmq_msg_send(&msg_t, m_socket, 
                              msg_list.size() > 1 ? ZMQ_SNDMORE : 0);
            if (rc < 0)
                return rc;

            zmq_msg_close(&msg_t);
            msg_list.pop_front();
        }
        return 0;
    }

    int Recv (void* socket)
    {
        ZMQ_ASSERT(m_parts.empty());
        ZMQ_ASSERT(socket);

        while(true) {
            m_parts.push_back(zmq_msg_t());
            zmq_msg_t& part = msg_list.back();
            zmq_msg_init(&part);
            rc = zmq_msg_recv(&part, socket, 0);
            if (rc < 0) {
                clear();
                return rc;
            }
        }

        return 0;
    }

    size_t Size()
    {
        return m_parts.size();
    }

    void Clear()
    {
        while (!m_parts.empty())
        {
            zmq_msg_t& msg_t = msg_list.front();
            zmq_msg_close(&msg_t);
            msg_list.pop_front();
        }
    }

private:
    std::list<zmq_msg_t> m_parts;
};

} // mdp


#endif // MDP_MSG_H

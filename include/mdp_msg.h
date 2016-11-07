#ifndef MDP_MSG_H
#define MDP_MSG_H

#include <vector>
#include <string>
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

    void PushFront (const std::string& data)
    {
        int rc = 0;
        m_parts.insert(m_parts.begin(), zmq_msg_t());
        zmq_msg_t& part = m_parts.front();

        if (data.empty())
            rc = zmq_msg_init(&part);
        else
            rc = zmq_msg_init_data(&part, data.c_str(),
                                   data.size(), 0, 0);
        ZMQ_ASSERT(rc == 0);

        return;
    }

    void PushBack (const std::string& data)
    {
        int rc = 0;
        m_parts.push_back(zmq_msg_t());
        zmq_msg_t& part = m_parts.back();
        if (data.empty())
            rc = zmq_msg_init(&part);
        else
            rc = zmq_msg_init_data(&part, data.c_str(),
                                   data.size(), 0, 0);
        ZMQ_ASSERT(rc == 0);

        return;
    }

    std::string PopFront ()
    {
        if (m_parts.empty())
            return 0;

        zmq_msg_t& part = m_parts.front();
        std::string data((char*)zmq_msg_data(&part));
        zmq_msg_close(&part);
        m_parts.erase(m_parts.begin());

        return data;
    }

    int Send (void* socket)
    {
        ZMQ_ASSERT (socket);
        ZMQ_ASSERT (!m_parts.empty());

        int rc = 0;
        int i  = 0;
        int part_size = (int)m_parts.size();
        for (; i < part_size; ++i)
        {
            rc = zmq_msg_send (&m_parts[i], socket, (i < part_size - 1) ? ZMQ_SNDMORE : 0);
            if (rc < 0)
                break;

            zmq_msg_close (&m_parts[i]);
        }
        m_parts.erase(m_parts.begin(), m_parts.begin() + i);

        return rc;
    }

    int Recv (void* socket)
    {
        int rc = 0;
        ZMQ_ASSERT (m_parts.empty ());
        ZMQ_ASSERT (socket);

        int more = 1;
        while (more)
        {
            m_parts.push_back (zmq_msg_t());
            zmq_msg_t& part = m_parts.back();
            rc = zmq_msg_init (&part);
            ZMQ_ASSERT (rc == 0);
            rc = zmq_msg_recv (&part, socket, 0);
            if (rc == -1)
            {
                zmq_msg_close (&part);
                m_parts.erase (m_parts.begin() + m_parts.size() - 1);
                return rc;
            }

            more = zmq_msg_more (&part);
        }

        return 0;
    }

    size_t  Size() { return m_parts.size();}
    void    Clear()
    {
        for (int i = 0; i < (int)m_parts.size(); ++i)
        {
            zmq_msg_t& part = m_parts[i];
            zmq_msg_close(&part);
        }

        m_parts.clear();
    }

private:
    std::vector<zmq_msg_t> m_parts;
};

} // mdp


#endif // MDP_MSG_H

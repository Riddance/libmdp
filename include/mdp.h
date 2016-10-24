#ifndef _MDP_H_
#define _MDP_H_

//Majordomo protocol
namspace mdp {

#ifndef NDEBUG
#   define ZMQ_ASSERT(expression) assert(expression)
#else
#   define ZMQ_ASSERT(expression) (void)(expression)
#endif

#define MDPC_CLIENT         "MDPC0X"

//  MDP/Client commands, as strings
#define MDPC_REQUEST        "\001"
#define MDPC_REPORT         "\002"
#define MDPC_NAK            "\003"

static char *mdpc_commands [] = {
    NULL, "REQUEST", "REPORT", "NAK",
};

//  This is the version of MDP/Worker we implement
#define MDPW_WORKER         "MDPW0X"

//  MDP/Worker commands, as strings
#define MDPW_READY          "\001"
#define MDPW_REQUEST        "\002"
#define MDPW_REPORT         "\003"
#define MDPW_HEARTBEAT      "\004"
#define MDPW_DISCONNECT     "\005"

static char *mdpw_commands [] = {
    NULL, "READY", "REQUEST", "REPORT", "HEARTBEAT", "DISCONNECT"
};

inline const char* err_msg()
{
    return zmq_strerror(zmq_errno());
}

inline const int err_code()
{
    return zmq_errno();
}

}

#endif

#ifndef MDP_DEFINE_H
#define MDP_DEFINE_H

#include "zmq.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
// #include "mdp_msg.h"

//Majordomo protocol
#ifndef NDEBUG
#   define MDP_ASSERT(expression) assert(expression)
#else
#   define MDP_ASSERT(expression) (void)(expression)
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

#define HEARTBEAT_LIVENESS  5
#define HEARTBEAT_INTERVAL  2500
#define HEARTBEAT_EXPIRY    HEARTBEAT_INTERVAL * HEARTBEAT_LIVENESS

namespace mdp {

int64_t
mdp_time (void)
{
#if defined (__UNIX__)
    struct timeval tv;
    gettimeofday (&tv, NULL);

    return (int64_t) ((int64_t) tv.tv_sec * 1000 + (int64_t) tv.tv_usec / 1000);
#elif (defined (__WINDOWS__))
    FILETIME ft;
    GetSystemTimeAsFileTime (&ft);
    return s_filetime_to_msec (&ft);
#endif
}

} // mdp


#endif // MDP_DEFINE_H

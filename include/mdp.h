#ifndef _MDP_H_
#define _MDP_H_

#include "zmq.h"
#include "mdp_cli.h"
#include "mdp_brk.h"
#include "mdp_wrk.h"

namspace mdp {

inline const char* err_msg() {
    return zmq_strerror(zmq_errno());
}

inline const int err_code() {
    return zmq_errno();
}

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

}

#endif

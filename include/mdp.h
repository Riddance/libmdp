#ifndef _MDP_H_
#define _MDP_H_

//#include "zmq.h"
#include "mdp_cli.h"
#include "mdp_brk.h"
#include "mdp_wrk.h"

namspace mdp {

//use this get error detail
inline const char* err_msg() {
    return zmq_strerror(zmq_errno());
}

inline const int err_code() {
    return zmq_errno();
}

}

#endif

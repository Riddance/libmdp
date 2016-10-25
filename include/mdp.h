#ifndef _MDP_H_
#define _MDP_H_

#include "zmq.h"
#include "mdp_client.h"
#include "mdp_broker.h"
#include "mdp_worker.h"

namspace mdp {

inline const char* err_msg() {
    return zmq_strerror(zmq_errno());
}

inline const int err_code() {
    return zmq_errno();
}

}

#endif

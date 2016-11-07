#ifndef MDP_BRK_H
#define MDP_BRK_H

#include <string>

namespace mdp {

struct WorkerInfo
{
    WorkerInfo (std::string& identity, ServiceInfo * service_info = 0, int expiry = 0)
    {
       m_identity       = identity;
       m_service_info   = service_info;
       m_expire_at      = expiry;
    }

    std::string     m_identity;
    int             m_expire_at;
    ServiceInfo*    m_service_info;
};

struct ServiceInfo
{
    ServiceInfo (std::string& name)
    {
       m_name       = name;
    }

    std::string                 m_name;
    std::vector<WorkerInfo*>    m_worker_vec;
};

typedef WorkerInfo* (*RouteHandler)(std::string& route_data, ServiceInfo* service_info);

class BrkApi
{
public:
    BrkApi (){}
    virtual ~BrkApi (){}

    int  Init(const std::string& endpoint);
    void Start();
    void Stop();
    void SetRouteHandler(RouteHandler* route_handler);

private:
    int     Bind();
    int     Close();
    void    ProcessClientMessage(mdp::MdpMsg& mdp_message, std::string& sender);
    void    ProcessWorkerMessage(mdp::MdpMsg& mdp_message, std::string& sender);
    void    WorkersPurge();

    static
    WorkerInfo*     DefaultRoute(std::string& route_data, ServiceInfo* service_info);
    WorkerInfo*     WokerRequire(const std::string& sender);
    ServiceInfo*    ServiceRequire(const std::string& service_name, WorkerInfo* worker);
    void            DeleteWorker(WorkerInfo* worker);

private:
    void*                                   m_ctx;
    void*                                   m_socket;
    std::string                             m_endpoint;
    bool                                    m_quit;
    RouteHandler*                           m_route_handler;
    std::map<std::string, ServiceInfo*>     m_services_map;  //  key:service_name value service
    std::map<std::string, WorkerInfo*>      m_workers_map;    // key:addr value:worker
};

} // mdp

#endif // MDP_BRK_H

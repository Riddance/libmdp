#include "mdp_define.h"
#include "mdp_brk.h"

namespace mdp {

BrkApi::BrkApi()
{
    m_quit          = false;
    m_ctx           = NULL;
    m_socket        = NULL;
    m_quit          = false;
    m_route_handler = NULL;
}

BrkApi::~BrkApi()
{
    Close();
}

int BrkApi::Init(const std::string& endpoint)
{
    m_ctx = zmq_ctx_new();
    if (m_ctx == NULL)
        return -1;

    return Bind();
}

int BrkApi::Bind()
{
    MDP_ASSERT(m_socket == NULL);
    m_socket = zmq_socket(m_ctx, ZMQ_ROUTER);
    if (socket_ == NULL)
        return -1;

    int rc = zmq_bind(m_socket, m_endpoint);
    if (rc != 0)
        return rc;

    return 0;
}

void BrkApi::Close()
{
    if (m_socket != 0)
    {
        rc = zmq_close(m_socket);
        MDP_ASSERT(rc == 0);
        m_socket = NULL;
    }

    if (m_ctx != 0)
    {
        rc = zmq_ctx_destroy(m_ctx);
        MDP_ASSERT(rc == 0);
        m_ctx = NULL;
    }

    return;
}

void BrkApi::Start()
{
    while (!m_quit)
    {
        int now = mdp::mdp_time();
        zmq_pollitem_t items [] = {m_socket, 0, ZMQ_POLLIN, 0};
        int time_out = heartbeat_at - now;
        rc = zmq_poll(items, 1, ZMQ_POLL_MSEC);
        MDP_ASSERT(rc == 0);

        if (items[0].revents & ZMQ_POLLIN)
        {
            mdp::MdpMsg mdp_message;
            rc = mdp_message.Recv(m_socket);
            if (rc != 0)
                return rc;

            std::string sender = mdp_message.PopFront();
            mdp_message.PopFront();

            std::string header = mdp_message.PopFront();
            if (header == std::string(MDPC_CLIENT))
            {
                ProcessClientMessage(mdp_message, sender);
            }
            else if (header == std::string(MDPW_WORKER))
            {
                ProcessWorkerMessage(mdp_message, sender);
            }
            else
            {
                MDP_ASSERT(0);
            }
        }

        WorkersPurge();
    }
}

void BrkApi::ProcessClientMessage(mdp::MdpMsg& mdp_message, std::string& sender)
{
    int rc = 0;
    MDP_ASSERT (mdp_message.size () >= 2);
    MDP_ASSERT (m_socket != NULL);

    std::string service_name    = mdp_message.PopFront();
    ServiceInfo*   service_info = m_services_map[service_name];
    if (service_info != NULL)
    {
        std::string route_data  = mdp_message.PopFront();
        WorkerInfo* worker_info = m_route_handler(route_data, service_info);
        mdp_message.PushFront("");
        mdp_message.PushFront(worker_info.m_identity);
        mdp_message.Send(m_socket);
    }

    return;
}

void BrkApi::ProcessWorkerMessage(mdp::MdpMsg& mdp_message, std::string& sender)
{
    int rc = 0;
    std::string cmd = mdp_message.PopFront();
    WorkerInfo* worker = WokerRequire(sender);
    MDP_ASSERT(worker != NULL);

    if (cmd == std::string(MDPW_READY))
    {
        std::string service_name = mdp_message.PopFront();
        ServiceInfo*   service   = ServiceRequire(service_name, worker);
        MDP_ASSERT (service != NULL);
    }
    else if (cmd == std::string(MDPW_REPLY))
    {
        std::string client = mdp_message.PopFront();
        std::string empty  = mdp_message.PopFront();

        mdp::MdpMsg send_msg;
        send_msg.PushFront(worker->m_service->m_name);
        send_msg.PushFront(MDPC_CLIENT);
        send_msg.PushFront("");
        send_msg.PushFront(client);
        send_msg.Send(m_socket);
    }
    else if (cmd == std::string(MDPW_HEARTBEAT))
    {
        worker->m_expire_at = mdp::mdp_time() + HEARTBEAT_INTERVAL;
    }
    else if (cmd == std::string(MDPW_DISCONNECT))
    {
        DeleteWorker(worker);
    }

    return;
}

void BrkApi::WorkersPurge()
{
    int time_now = mdp::mdp_time();
    std::vector<WorkerInfo*> delete_workers;

    std::map<std::string, WorkerInfo*>::iterator it;
    for(it = m_workers_map.begin(); it != m_workers_map.end(); ++i)
    {
        WorkerInfo* worker_info = it->second;
        if (time_now > worker_info.m_expire_at)
        {
            delete_workers.push_back(worker_info);
        }
    }

    for (int i = 0; i < (int)delete_workers.size(); ++i)
    {
        DeleteWorker(delete_workers[i]);
    }

    return;
}

WorkerInfo* BrkApi::DefaultRoute(std::string& route_data, ServiceInfo* service_info)
{
    route_data.empty();

    WorkerInfo* worker_info = NULL;
    if (service_info.m_worker_vec.size() >= 1)
    {
        worker_info = service_info.m_worker_vec[0];
        if (service_info.m_worker_vec.size() > 1)
        {
            service_info.m_worker_vec.erase(m_worker_vec.begin());
            service_info.m_worker_vec.push_back(worker_info);
        }
    }

    return worker_info;
}

WorkerInfo* BrkApi::WokerRequire(const std::string& sender)
{
    WorkerInfo* worker = m_workers_map[sender];
    if (worker == NULL)
    {
        worker = new WorkerInfo(sender, 0, mdp_time() + HEARTBEAT_EXPIRY);
        MDP_ASSERT (worker != NULL);
        m_workers_map[sender] = worker;
    }

    return worker;
}

ServiceInfo* BrkApi::ServiceRequire(const std::string& service_name, WorkerInfo* worker)
{
    ServiceInfo*   service      = m_services_map[service_name];
    if (service == NULL)
    {
        service = new ServiceInfo(service_name);
        m_services_map[service_name] = service;
    }

    std::vector<WorkerInfo *>::iterator it = std::find(service.m_workers.begin (), service.m_workers.end ());
    if (it == service.m_workers.end())
        service.m_workers.push_back(worker);

    return service;
}

void BrkApi::DeleteWorker(WorkerInfo* worker)
{
    MDP_ASSERT (worker != NULL);
    ServiceInfo* service = worker->m_service;

    MDP_ASSERT (service != NULL);
    std::vector<WorkerInfo *>::iterator it =
    std::find(service->m_workers.begin(), service->m_workers.end());

    service->m_workers.erase(it);
    m_workers_map.erase(worker->m_identity);
    delete worker;
    worker = NULL;

    return;
}

} // mdp

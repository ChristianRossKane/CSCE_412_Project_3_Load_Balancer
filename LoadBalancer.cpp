///////////////////////////////////////////////////////////////////////////////
// LoadBalancer Class : Function Bodies
///////////////////////////////////////////////////////////////////////////////

#include "LoadBalancer.h"
#include <iostream>
#include <ctime>
#include <filesystem>

///////////////////////////////////////////////////////////////////////////////
// Initializing Globals.
///////////////////////////////////////////////////////////////////////////////
const int       s_nClockStartTime = 0;
int             s_nCurrentClockTime = 0;
int             s_nClockEndTime = 10000;
int             s_nServerCount = 10;
std::ofstream   s_fileLog;

///////////////////////////////////////////////////////////////////////////////
// Names for each Request::JobType, indexed by the enum value. Used for
// logging only.
///////////////////////////////////////////////////////////////////////////////
static const char* const s_astrJobTypeNames[] = {   "Character", 
                                                    "Processing", 
                                                    "Streaming" };

///////////////////////////////////////////////////////////////////////////////
//  ● Webserver(s)
//      o Takes requests from the Load Balancer
//      o Processes the requests
//      o Asks for another
///////////////////////////////////////////////////////////////////////////////
bool                    
WebServer::ProcessingRequest()
{
    //
    // If the current time is BEFORE the finish time, then we are still
    // processing the request.
    return s_nCurrentClockTime < m_nRequestWillFinish;
}

void                    
WebServer::ProcessRequest(Request req)
{
    //
    // Store the request.
    m_RequestBeingProcessed = req;

    //
    // State when the request was received. (Assuming that it can immediately
    // start handling the request.)
    m_nRequestReceived = s_nCurrentClockTime;

    //
    // Say when we will finish the request. Subtract 1 since we assume that the
    // server immediately starts processing it.
    m_nRequestWillFinish = m_nRequestReceived + req.m_nProcessingDuration - 1;
}

void
WebServer::UpdateLog()
{
    if (ProcessingRequest()) {
        s_fileLog << "    Server ACTIVE: processing " << s_astrJobTypeNames[m_RequestBeingProcessed.m_eJobType]
                  << " request (" << m_RequestBeingProcessed.m_strIPIn 
                  << " -> " << m_RequestBeingProcessed.m_strIPOut
                  << "), finishes at cycle " << m_nRequestWillFinish << std::endl;
    } else {
        s_fileLog << "    Server IDLE" << std::endl;
    }
}




///////////////////////////////////////////////////////////////////////////////
//  ● Load Balancer
//      o Queue of requests
//      o Keeps track of time.
///////////////////////////////////////////////////////////////////////////////
LoadBalancer* LoadBalancer::sm_this = nullptr;
LoadBalancer::LoadBalancer() : 
    m_Servers(s_nServerCount, WebServer()),
    m_CharacterLoadBalancer(Request::JobType::eCharacter), 
    m_ProcessingLoadBalancer(Request::JobType::eProcessing), 
    m_StreamingLoadBalancer(Request::JobType::eStreaming)
{
    //
    // Debug Code:
    std::cout << "Load Balancer instantiated." << std::endl;

    //
    // Assign the static singleton.
    sm_this = this;

    //
    // Create the log. Each execution gets its own timestamped file, written
    // into a "logs" folder so they don't clutter the main directory.
    std::filesystem::create_directories("logs");
    std::time_t tNow = std::time(nullptr);
    std::tm* pTM = std::localtime(&tNow);
    char szFilename[64];
    std::strftime(szFilename, sizeof(szFilename), "logs/load_balancer_%Y%m%d_%H%M%S.log", pTM);
    s_fileLog.open(szFilename);

    s_fileLog << "===================================================================" << std::endl;
    s_fileLog << "CSCE 412 Load Balancer Simulation Log" << std::endl;
    s_fileLog << "Servers: " << s_nServerCount << " | Clock Cycles: " << s_nClockEndTime << std::endl;
    s_fileLog << "Task processing time range: 1 - 75 cycles" << std::endl;
    s_fileLog << "===================================================================" << std::endl;
}

LoadBalancer::~LoadBalancer()
{
    //
    // Debug Code:
    std::cout << "Load Balancer destroyed." << std::endl;

    //
    // Close the log file.
    s_fileLog.close();
}

void
LoadBalancer::StartSimulation()
{
    //
    // Capture starting state for the end-of-run summary. The initial full
    // queue was already delegated into the three internal balancers before
    // this call, so their combined size right now is the starting size.
    int nStartingQueueSize = (int)(m_CharacterLoadBalancer.m_qRequests.size() +
                                   m_ProcessingLoadBalancer.m_qRequests.size() +
                                   m_StreamingLoadBalancer.m_qRequests.size());
    s_fileLog << "\nStarting queue size: " << nStartingQueueSize << std::endl;
    s_fileLog << "Starting web server count: " << m_Servers.size() << "\n" << std::endl;

    //
    // "Keeps track of time"; i.e. the simulation will be run here.
    // Create a log of 10 servers running for 10000 clock cycles.
    for (s_nCurrentClockTime = s_nClockStartTime;
         s_nCurrentClockTime < s_nClockEndTime;
         ++s_nCurrentClockTime) {
        //
        // Recall that we may want to add a new request. Let's say that 10% of
        // the time we generate a new request.
        if (GenerateRandomNumber(1, 10) == 1) {
            Request newReq;
            s_fileLog << "[Cycle " << s_nCurrentClockTime 
                      << "] New " << s_astrJobTypeNames[newReq.m_eJobType]
                      << " request generated (" << newReq.m_strIPIn 
                      << " -> " << newReq.m_strIPOut
                      << ", duration " << newReq.m_nProcessingDuration << ")" << std::endl;
            DelegateRequest(newReq);
        }
        m_CharacterLoadBalancer.Tick();
        m_ProcessingLoadBalancer.Tick();
        m_StreamingLoadBalancer.Tick();

        //
        // Periodic status snapshot rather than every cycle, to keep the log
        // readable across a 10000-cycle run.
        if (s_nCurrentClockTime % 100 == 0)
            UpdateLog();
    }

    //
    // End-of-run summary.
    int nEndingQueueSize = (int)(m_CharacterLoadBalancer.m_qRequests.size() +
                                 m_ProcessingLoadBalancer.m_qRequests.size() +
                                 m_StreamingLoadBalancer.m_qRequests.size());
    int nActiveServers = 0;
    for (auto& server : m_Servers) {
        if (server.ProcessingRequest())
            ++nActiveServers;
    }
    int nInactiveServers = (int)m_Servers.size() - nActiveServers;

    s_fileLog << "\n===================================================================" << std::endl;
    s_fileLog << "Simulation Summary" << std::endl;
    s_fileLog << "===================================================================" << std::endl;
    s_fileLog << "Starting queue size:          " << nStartingQueueSize << std::endl;
    s_fileLog << "Ending queue size:            " << nEndingQueueSize << std::endl;
    s_fileLog << "Starting web servers:         " << m_nStartingServerCount << std::endl;
    s_fileLog << "Ending web servers:           " << m_Servers.size() << std::endl;
    s_fileLog << "Total web servers added:      " << m_nServersAdded << std::endl;
    s_fileLog << "Total web servers deleted:    " << m_nServersDeleted << std::endl;
    s_fileLog << "Random task duration:         1 - 75 cycles" << std::endl;
    s_fileLog << "Remaining requests in queue:  " << nEndingQueueSize << std::endl;
    s_fileLog << "Active servers:                " << nActiveServers << std::endl;
    s_fileLog << "Inactive servers:              " << nInactiveServers << std::endl;
    s_fileLog << "Rejected/discarded requests:   0 (queue is never capped; dynamic scaling keeps it bounded instead)" << std::endl;
    s_fileLog << "===================================================================" << std::endl;
}

void
LoadBalancer::UpdateLog()
{
    //
    // Should write to the log.
    s_fileLog << "--- Status at cycle " << s_nCurrentClockTime << " ---" << std::endl;

    m_CharacterLoadBalancer.UpdateLog();
    m_ProcessingLoadBalancer.UpdateLog();
    m_StreamingLoadBalancer.UpdateLog();

    for (auto& server : m_Servers)
        server.UpdateLog();
}

void                    
LoadBalancer::DelegateRequest(Request& req)
{
    //
    // Should pass each request to the appropriate balancer:
    switch (req.m_eJobType) {
        case Request::JobType::eCharacter:
        {
            m_CharacterLoadBalancer.AddRequest(req);
            break;
        }
        case Request::JobType::eProcessing:
        {
            m_ProcessingLoadBalancer.AddRequest(req);
            break;
        }
        case Request::JobType::eStreaming:
        {
            m_StreamingLoadBalancer.AddRequest(req);
            break;
        }
    }
}




///////////////////////////////////////////////////////////////////////////
// Internal Load Balancer Class:
//  o A Load Balancer for each Job Type.
///////////////////////////////////////////////////////////////////////////
LoadBalancer::InternalLoadBalancer::InternalLoadBalancer(Request::JobType eJobType)
{
    m_eLoadBalancerType = eJobType;
}


void
LoadBalancer::InternalLoadBalancer::AddRequest(Request& req)
{
    m_qRequests.push(req);
}

void
LoadBalancer::InternalLoadBalancer::Tick()
{
    //
    // Handles each clock cycle. Should only assign a single Request per cycle
    // to ensure that each Internal Balancer does not get preference over
    // another (i.e. so one doesn't hog all of the servers.)
    Servers& servers = LoadBalancer::sm_this->m_Servers;

    //
    // Dynamic scaling: grow the shared server pool when this balancer's
    // queue is backing up faster than the pool can absorb it, shrink it when
    // this balancer has nothing pending and an idle server can be spared.
    // Capped at 4x the initial server count so the pool can't grow unbounded.
    if (m_qRequests.size() > servers.size() && servers.size() < (size_t)(s_nServerCount * 4)) {
        servers.push_back(WebServer());
        ++LoadBalancer::sm_this->m_nServersAdded;
        s_fileLog << "[Cycle " << s_nCurrentClockTime 
                  << "] Scaled UP (" << s_astrJobTypeNames[m_eLoadBalancerType]
                  << "): added a server, total now " << servers.size() << std::endl;
    } else if (m_qRequests.empty() && servers.size() > 1) {
        for (auto it = servers.begin(); it != servers.end(); ++it) {
            if (!it->ProcessingRequest()) {
                servers.erase(it);
                ++LoadBalancer::sm_this->m_nServersDeleted;
                s_fileLog << "[Cycle " << s_nCurrentClockTime 
                          << "] Scaled DOWN (" << s_astrJobTypeNames[m_eLoadBalancerType]
                           << "): removed an idle server, total now " << servers.size() << std::endl;
                break;
            }
        }
    }

    //
    // Nothing to assign if the queue is empty.
    if (m_qRequests.empty())
        return;

    for(auto& server : servers) {
        if (!server.ProcessingRequest()) {
            s_fileLog << "[Cycle " << s_nCurrentClockTime 
                      << "] Assigned " << s_astrJobTypeNames[m_eLoadBalancerType]
                      << " request (" << m_qRequests.front().m_strIPIn 
                      << " -> " << m_qRequests.front().m_strIPOut
                      << ") to a server" << std::endl;
            server.ProcessRequest(m_qRequests.front());
            m_qRequests.pop();
            return;
        }
    }
}

void
LoadBalancer::InternalLoadBalancer::UpdateLog()
{
    s_fileLog << "    " << s_astrJobTypeNames[m_eLoadBalancerType] 
              << " queue: " << m_qRequests.size()
              << " pending" << std::endl;
}
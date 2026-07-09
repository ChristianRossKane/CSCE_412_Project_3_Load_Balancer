///////////////////////////////////////////////////////////////////////////////
// LoadBalancer Class : Function Bodies
///////////////////////////////////////////////////////////////////////////////

#include "LoadBalancer.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
// Initializing Globals.
///////////////////////////////////////////////////////////////////////////////
const int       s_nClockStartTime = 0;
int             s_nCurrentClockTime = 0;
int             s_nClockEndTime = 10000;
int             s_nServerCount = 10;
std::ofstream   s_fileLog;

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
    //
    // TBD
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
    // Create the log.
    // TBD
}

LoadBalancer::~LoadBalancer()
{
    //
    // Debug Code:
    std::cout << "Load Balancer destroyed." << std::endl;

    //
    // Close the log file.
    // TBD
}

void           
LoadBalancer::StartSimulation()
{
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
            DelegateRequest(newReq);
        }
        m_CharacterLoadBalancer.Tick();
        m_ProcessingLoadBalancer.Tick();
        m_StreamingLoadBalancer.Tick();
    }
}

void            
LoadBalancer::UpdateLog()
{
    //
    // Should write to the log.
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
    for(auto& server : servers) {
        if (!server.ProcessingRequest()) {
            server.ProcessRequest(m_qRequests.front());
            m_qRequests.pop();
            return;
        }            
    }
}

void
LoadBalancer::InternalLoadBalancer::UpdateLog()
{
    // 
    // TBD
}
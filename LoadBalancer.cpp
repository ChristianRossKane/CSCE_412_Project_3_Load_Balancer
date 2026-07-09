///////////////////////////////////////////////////////////////////////////////
// LoadBalancer Class : Function Bodies
///////////////////////////////////////////////////////////////////////////////

#include "LoadBalancer.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//  ● Requests - generates random IP addresses, both in and out, and random 
//    times for each request to be processed.
//      o IP in
//      o IP out
//      o Time (integer)
//      o Job Type (Character, p (processing), or s (streaming))
///////////////////////////////////////////////////////////////////////////////
Request::Request()
{
    //
    // Randomly create each member:

    // ...
}




///////////////////////////////////////////////////////////////////////////////
//  ● Load Balancer
//      o Queue of requests
//      o Keeps track of time.
///////////////////////////////////////////////////////////////////////////////
LoadBalancer::LoadBalancer() : 
    m_CharacterLoadBalancer(Request::JobType::eCharacter), 
    m_ProcessingLoadBalancer(Request::JobType::eProcessing), 
    m_StreamingLoadBalancer(Request::JobType::eStreaming)
{
    //
    // Debug Code:
    std::cout << "Load Balancer instantiated." << std::endl;

    //
    // Create the log.
}

LoadBalancer::~LoadBalancer()
{
    //
    // Debug Code:
    std::cout << "Load Balancer destroyed." << std::endl;

    //
    // Close the log file.
}

void           
LoadBalancer::StartSimulation()
{
    //
    // "Keeps track of time"; i.e. the simulation will be run here.
    // Create a log of 10 servers running for 10000 clock cycles
    for (int i = 1; i <= s_nClockEndTime; ++i) {
        if (i % 1000 == 0)
        std::cout << "Cycle " << i << std::endl;

        //
        // ...
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
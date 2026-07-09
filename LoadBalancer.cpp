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

}




///////////////////////////////////////////////////////////////////////////////
//  ● Load Balancer
//      o Queue of requests
//      o Keeps track of time.
///////////////////////////////////////////////////////////////////////////////
LoadBalancer::LoadBalancer(Requests qInitial)
{
    //
    // Debug Code:
    std::cout << "Load Balancer instantiated." << std::endl;

    //
    // Initialize:
    m_qAllRequests = qInitial;
}

LoadBalancer::~LoadBalancer()
{
    //
    // Debug Code:
    std::cout << "Load Balancer destroyed." << std::endl;
}

void            
LoadBalancer::AddRequest(Request& req)
{
    m_qAllRequests.push(req);
}
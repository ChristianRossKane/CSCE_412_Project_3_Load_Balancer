/**
 * @file main.cpp
 * @brief Driver program for the CSCE 412 Load Balancer simulation.
 */

#include "LoadBalancer.h"
#include <iostream>

/**
 * @brief Entry point. Prompts for a server count and clock-cycle run length,
 *        builds an initial full queue of servers * 100 random Requests, and
 *        runs the simulation.
 * @return 0 on successful completion.
 */
int main()
{
    ///////////////////////////////////////////////////////////////////////////
    // "The main program should set up the number of servers (user input), the
    // time you want to run the load balancer (user input), and generate a full
    // queue (usually servers * 100)."
    //
    // You will need to add new requests at random times to simulate new
    // requests after the initial full queue you set up.
    ///////////////////////////////////////////////////////////////////////////

    //
    // Intro messages:
    std::cout << "****************************************************************************************************" << std::endl;
    std::cout << "\nWelcome to Christian Kane's CSCE 412 Cloud Computing Load Balancer Simulation!" << std::endl;
    std::cout << "****************************************************************************************************" << std::endl;

    //
    // Get the number of servers.
    std::cout << "\nEnter a number of servers to simulate requests between: ";
    std::cin >> s_nServerCount;

    //
    // Get the total simulated clock cycles:
    std::cout << "\nEnter a number of simulated clock cycles: ";
    std::cin >> s_nClockEndTime;

    //
    // "...generate a full queue (usually servers * 100)"
    LoadBalancer lb;
    for (size_t i = 0; i < (s_nServerCount * 100); ++i) {
        //
        // All data for the Request should be handled during construction.
        Request req;
        lb.DelegateRequest(req);
    }
    lb.StartSimulation();

    return 0;           
}
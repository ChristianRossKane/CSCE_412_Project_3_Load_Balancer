#include "LoadBalancer.h"
#include <iostream>

int main()
{
    ///////////////////////////////////////////////////////////////////////////
    // "The main program should set up the number of servers (user input), the 
    // time you want to run the load balancer (user input), and generate a full
    // queue (usually servers * 100)."
    // 
    // ...
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
    Requests qInitial;
    for (size_t i = 0; i < (s_nServerCount * 100); ++i)
        //
        // All data for the Request should be handled during construction.
        qInitial.push(Request());

    //
    // Initialize locals for the simulation.
    LoadBalancer lb(qInitial);

    //
    // Create a log of 10 servers running for 10000 clock cycles
    for (int i = 1; i <= s_nClockEndTime; ++i) {
        if (i % 1000 == 0)
        std::cout << "Cycle " << i << std::endl;

        //
        // ...
    }

    return 0;           
}
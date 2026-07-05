///////////////////////////////////////////////////////////////////////////////
// LoadBalancer Class : Header Definition
// "Load balancers are available commercially, but in this project, you will 
//  build one that has:
//  ● Requests - generates random IP addresses, both in and out, and random 
//    times for each request to be processed.
//      o IP in
//      o IP out
//      o Time (integer)
//      o Job Type (Character, p (processing), or s (streaming))
//  ● Webserver(s)
//      o Takes requests from the Load Balancer
//      o Processes the requests
//      o Asks for another
//  ● Load Balancer
//      o Queue of requests
//      o Keeps track of time."
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  ● Requests - generates random IP addresses, both in and out, and random 
//    times for each request to be processed.
//      o IP in
//      o IP out
//      o Time (integer)
//      o Job Type (Character, p (processing), or s (streaming))
///////////////////////////////////////////////////////////////////////////////
struct Request
{
    // ...
};




///////////////////////////////////////////////////////////////////////////////
//  ● Requests - generates random IP addresses, both in and out, and random 
//    times for each request to be processed.
//      o IP in
//      o IP out
//      o Time (integer)
//      o Job Type (Character, p (processing), or s (streaming))
///////////////////////////////////////////////////////////////////////////////
class WebServer
{
public:

    // ...

private:

    // ...

};




///////////////////////////////////////////////////////////////////////////////
//  ● Load Balancer
//      o Queue of requests
//      o Keeps track of time.
///////////////////////////////////////////////////////////////////////////////
class LoadBalancer
{
public:
    LoadBalancer();
    ~LoadBalancer();

    // ...

private:

    // ...

};
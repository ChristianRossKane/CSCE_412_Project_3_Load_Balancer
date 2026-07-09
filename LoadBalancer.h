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

#include <queue>

///////////////////////////////////////////////////////////////////////////////
// Global Start and End Times for the Simluation (the end time should be
// specified at the start of main())
///////////////////////////////////////////////////////////////////////////////
static const int    s_nClockStartTime = 0;
static int          s_nClockEndTime = 10000;
static int          s_nServerCount = 10;

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
    Request();
    
    //
    // Request::JobType Enum Definition:
    enum JobType {
        eCharacter = 0,
        eProcessing,
        eStreaming
    };

    //
    // Request Data:
    std::string     m_strIPIn;
    std::string     m_strIPOut;
    int             m_nTimeToBeProcessed; // Should be in range s_nClockStartTime -> s_nClockEndTime
    JobType         m_eJobType;
};
typedef std::queue<Request> Requests;


///////////////////////////////////////////////////////////////////////////////
//  ● Webserver(s)
//      o Takes requests from the Load Balancer
//      o Processes the requests
//      o Asks for another
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
    LoadBalancer(Requests qInitial);
    ~LoadBalancer();

    void            AddRequest(Request& req);

private:

    Requests        m_qAllRequests;
};
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
#include <random>
#include <fstream>

///////////////////////////////////////////////////////////////////////////////
// Globals for use throughout the whole program.
///////////////////////////////////////////////////////////////////////////////
extern const int            s_nClockStartTime;
extern int                  s_nCurrentClockTime;
extern int                  s_nClockEndTime;
extern int                  s_nServerCount;
extern std::ofstream        s_fileLog;

///////////////////////////////////////////////////////////////////////////////
// Helper to generate random numbers.
///////////////////////////////////////////////////////////////////////////////
inline int 
GenerateRandomNumber(int nLowerBound, int nUpperBound)
{
    static std::random_device SEED;
    static std::mt19937 gen(SEED());
    std::uniform_int_distribution<int> dist(nLowerBound, nUpperBound);
    return dist(gen);
}

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
    Request()
    {
        //
        // Randomly create each member:
        m_eJobType = JobType(GenerateRandomNumber(0, 2));
        m_nProcessingDuration = GenerateRandomNumber(1, 100);
        //m_strIPIn = ;
        //m_strIPOut = ;
    }
    
    //
    // Request::JobType Enum Definition:
    enum JobType {
        eCharacter = 0,
        eProcessing,
        eStreaming
    };

    //
    // Request Data:
    std::string             m_strIPIn;
    std::string             m_strIPOut;
    int                     m_nProcessingDuration; // random int 1-100
    JobType                 m_eJobType;
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
    bool                    ProcessingRequest();
    void                    ProcessRequest(Request req);
    void                    UpdateLog();

private:

    Request                 m_RequestBeingProcessed;
    int                     m_nRequestReceived;
    int                     m_nRequestWillFinish;
};
typedef std::vector<WebServer> Servers;




///////////////////////////////////////////////////////////////////////////////
//  ● Load Balancer
//      o Queue of requests
//      o Keeps track of time.
///////////////////////////////////////////////////////////////////////////////
class LoadBalancer
{
    friend class InternalLoadBalancer;
public:

    LoadBalancer();
    ~LoadBalancer();

    void                    StartSimulation();
    void                    DelegateRequest(Request& req);
    void                    UpdateLog();

private:

    ///////////////////////////////////////////////////////////////////////////
    // Internal Load Balancer Class:
    //  o A Load Balancer for each Job Type.
    ///////////////////////////////////////////////////////////////////////////
    class InternalLoadBalancer
    {
        friend LoadBalancer;
    public:
    
        InternalLoadBalancer(Request::JobType eJobType);

        void                AddRequest(Request& req);
        void                Tick();
        void                UpdateLog();

    private:
        Requests            m_qRequests;
        Request::JobType    m_eLoadBalancerType;
    };

    ///////////////////////////////////////////////////////////////////////////
    // Members:
    ///////////////////////////////////////////////////////////////////////////
    static LoadBalancer*    sm_this;
    Servers                 m_Servers;
    InternalLoadBalancer    m_CharacterLoadBalancer;
    InternalLoadBalancer    m_ProcessingLoadBalancer;
    InternalLoadBalancer    m_StreamingLoadBalancer;
};
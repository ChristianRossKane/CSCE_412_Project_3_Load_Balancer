/**
 * @file LoadBalancer.h
 * @brief Header definitions for the CSCE 412 Load Balancer simulation.
 *
 * "Load balancers are available commercially, but in this project, you will
 *  build one that has:
 *  - Requests - generates random IP addresses, both in and out, and random
 *    times for each request to be processed.
 *      - IP in
 *      - IP out
 *      - Time (integer)
 *      - Job Type (Character, p (processing), or s (streaming))
 *  - Webserver(s)
 *      - Takes requests from the Load Balancer
 *      - Processes the requests
 *      - Asks for another
 *  - Load Balancer
 *      - Queue of requests
 *      - Keeps track of time."
 */

#include <queue>
#include <random>
#include <fstream>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////
// Globals for use throughout the whole program.
///////////////////////////////////////////////////////////////////////////////

/// The clock cycle the simulation begins at. Always 0.
extern const int            s_nClockStartTime;

/// The current clock cycle of the running simulation.
extern int                  s_nCurrentClockTime;

/// The clock cycle the simulation ends at (user-provided run length).
extern int                  s_nClockEndTime;

/// The number of web servers the simulation starts with (user-provided).
extern int                  s_nServerCount;

/// The open log file that every class in the simulation writes to.
extern std::ofstream        s_fileLog;

/**
 * @brief Generates a uniformly-distributed random integer in a closed range.
 * @param nLowerBound The inclusive lower bound of the random range.
 * @param nUpperBound The inclusive upper bound of the random range.
 * @return A random integer within [nLowerBound, nUpperBound].
 */
inline int
GenerateRandomNumber(int nLowerBound, int nUpperBound)
{
    static std::random_device SEED;
    static std::mt19937 gen(SEED());
    std::uniform_int_distribution<int> dist(nLowerBound, nUpperBound);
    return dist(gen);
}

/**
 * @brief A single simulated web request.
 *
 * Every member is randomly generated at construction time: a random source
 * and destination IP address, a random job type, and a random duration (in
 * clock cycles) that the request will occupy a WebServer for.
 */
struct Request
{
    /// Randomly generates every member of the request.
    Request()
    {
        //
        // Randomly create each member:
        m_eJobType = JobType(GenerateRandomNumber(0, 2));
        m_nProcessingDuration = GenerateRandomNumber(1, 75);
        std::ostringstream ossIn;
        ossIn << GenerateRandomNumber(0, 255) << "." <<
                 GenerateRandomNumber(0, 255) << "." <<
                 GenerateRandomNumber(0, 255) << "." <<
                 GenerateRandomNumber(0, 255);
        m_strIPIn = ossIn.str();
        std::ostringstream ossOut;
        ossOut << GenerateRandomNumber(0, 255) << "." <<
                  GenerateRandomNumber(0, 255) << "." <<
                  GenerateRandomNumber(0, 255) << "." <<
                  GenerateRandomNumber(0, 255);
        m_strIPOut = ossOut.str();
    }

    /// The kind of work a Request represents.
    enum JobType {
        eCharacter = 0,  ///< Character-oriented processing job.
        eProcessing,     ///< General processing job.
        eStreaming       ///< Streaming job.
    };

    //
    // Request Data:
    std::string             m_strIPIn;              ///< Randomly generated source IP address.
    std::string             m_strIPOut;              ///< Randomly generated destination IP address.
    int                     m_nProcessingDuration;   ///< random int 1-75: cycles this request occupies a server for.
    JobType                 m_eJobType;               ///< The random job type of this request.
};

/// A FIFO queue of pending Requests.
typedef std::queue<Request> Requests;

/**
 * @brief Simulates a single web server that processes one Request at a time.
 *
 * A WebServer is either idle or actively processing a Request. Once assigned
 * a Request via ProcessRequest(), it remains busy until the simulation clock
 * reaches the request's finish cycle, at which point ProcessingRequest()
 * reports it as idle again and it can be handed another Request.
 */
class WebServer
{
public:
    /**
     * @brief Reports whether this server is currently busy with a Request.
     * @return true if still processing a Request, false if idle.
     */
    bool                    ProcessingRequest();

    /**
     * @brief Assigns a new Request to this server for processing.
     * @param req The Request to begin processing.
     */
    void                    ProcessRequest(Request req);

    /// Writes this server's current active/idle status to the log.
    void                    UpdateLog();

private:

    Request                 m_RequestBeingProcessed;    ///< The Request currently assigned to this server.
    int                     m_nRequestReceived = -1;    ///< Clock cycle the current Request was received; -1 if never assigned.
    int                     m_nRequestWillFinish = -1;  ///< Clock cycle the current Request will finish; -1 means idle: always < s_nCurrentClockTime.
};

/// A pool of WebServer instances, shared by every LoadBalancer::InternalLoadBalancer.
typedef std::vector<WebServer> Servers;

/**
 * @brief Top-level load balancer that owns the shared server pool and routes
 *        every incoming Request to the InternalLoadBalancer for its job type.
 *
 * Internally, three InternalLoadBalancer instances each manage their own
 * queue for one Request::JobType (Character, Processing, Streaming), while
 * all three read from and dynamically resize the same shared WebServer pool.
 */
class LoadBalancer
{
    friend class InternalLoadBalancer;
public:

    /// Constructs the server pool and the three job-type balancers, and opens a new timestamped log file.
    LoadBalancer();

    /// Closes the log file.
    ~LoadBalancer();

    /// Runs the simulation from s_nClockStartTime to s_nClockEndTime, ticking every balancer each cycle.
    void                    StartSimulation();

    /**
     * @brief Routes a Request to the InternalLoadBalancer matching its job type.
     * @param req The Request to delegate.
     */
    void                    DelegateRequest(Request& req);

    /// Writes a full status snapshot (every balancer and every server) to the log.
    void                    UpdateLog();

private:

    ///////////////////////////////////////////////////////////////////////////
    // Internal Load Balancer Class:
    //  o A Load Balancer for each Job Type.
    ///////////////////////////////////////////////////////////////////////////
    /**
     * @brief Manages the request queue and dynamic server scaling for a single Request::JobType.
     *
     * Each InternalLoadBalancer owns its own Requests queue, but reads from
     * and resizes the shared LoadBalancer::m_Servers pool (via the enclosing
     * LoadBalancer::sm_this singleton) rather than owning servers itself.
     */
    class InternalLoadBalancer
    {
        friend LoadBalancer;
    public:

        /**
         * @brief Constructs a balancer responsible for a single job type.
         * @param eJobType The Request::JobType this balancer is responsible for.
         */
        InternalLoadBalancer(Request::JobType eJobType);

        /**
         * @brief Queues a Request for this balancer to later assign to a server.
         * @param req The Request to enqueue.
         */
        void                AddRequest(Request& req);

        /**
         * @brief Advances this balancer by one clock cycle.
         *
         * Scales the shared server pool up or down as needed based on this
         * balancer's queue depth, then assigns at most one queued Request to
         * an idle server (so no single job type can monopolize the pool in a
         * single cycle).
         */
        void                Tick();

        /// Writes this balancer's queue depth to the log.
        void                UpdateLog();

    private:
        Requests            m_qRequests;          ///< Pending Requests of this balancer's job type.
        Request::JobType    m_eLoadBalancerType;  ///< The job type this balancer is responsible for.
    };

    ///////////////////////////////////////////////////////////////////////////
    // Members:
    ///////////////////////////////////////////////////////////////////////////
    static LoadBalancer*    sm_this;                                  ///< Singleton pointer so InternalLoadBalancer::Tick() can reach the shared server pool.
    Servers                 m_Servers;                                ///< The shared pool of web servers, dynamically resized during the simulation.
    int                     m_nStartingServerCount = s_nServerCount;  ///< Server pool size at construction, recorded for the end-of-run summary.
    int                     m_nServersAdded = 0;                      ///< Total servers dynamically added over the simulation.
    int                     m_nServersDeleted = 0;                    ///< Total servers dynamically removed over the simulation.
    InternalLoadBalancer    m_CharacterLoadBalancer;                  ///< Handles Request::eCharacter jobs.
    InternalLoadBalancer    m_ProcessingLoadBalancer;                 ///< Handles Request::eProcessing jobs.
    InternalLoadBalancer    m_StreamingLoadBalancer;                  ///< Handles Request::eStreaming jobs.
};

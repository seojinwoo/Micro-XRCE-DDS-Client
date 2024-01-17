
#include "global.h"
#include "off_topic.h"

#ifdef __linux__

#else
LARGE_INTEGER getFILETIMEoffset();
int clock_gettime(struct timeval *tv);
#endif

void PubTask(char *ip, char *port, int index)
{
    zigbangUXR uxrPubOnly;
    uint32_t count = 0;
    threadRun.set_bit(index);

    if (uxrPubOnly.InitParticipant(ip, port, BUFFER_SIZE, BUFFER_SIZE, PARTICIPANT_ID | PARTICIPANT_ID_PUB) == false)
    {
        std::cout << "Init Participant Error for Pub" << std::endl;
    }
    else
    {
        // Link Topic
        {
            std::map<uint8_t, std::pair<std::string, std::string>>::iterator it;
            for (it = dicTopics.begin(); it != dicTopics.end(); it++)
            {
                uxrPubOnly.LinkPub(it->first, it->second.first.c_str(), it->second.second.c_str());
            }
        }

        // Send create entities message and wait its status
        if (uxrPubOnly.RegisterEntity() == false)
        {
            std::cout << "Error at RegisterEntity Pub" << std::endl;
        }
        else
        {
            bool connected = true;
            uint32_t count = 0;

            std::map<uint8_t, std::pair<std::string, std::string>>::iterator it;
            it = dicTopics.begin();

            threadIdle.set_bit(index);
            while (connected)
            {
                connected = uxr_run_session_time(&uxrPubOnly.session, 100);

                if (connected == false)
                {
                    break;
                }

                if (threadRun.get_bit(EXIT_PROGRAM_ID))
                {
                    connected = false;
                    break;
                }

                if (toPub.count() > 0)
                {
                    std::string ToServer = toPub.pop().c_str();

                    // Make Json object from ToServer
                    to_topic(ToServer, it->first, uxrPubOnly);
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
        }
    }

    std::cout << "Exit Pub Thread" << std::endl;

    threadRun.clear_bit(index);
    uxrPubOnly.Exit();
}

#ifdef __linux__

#else
#define BILLION 1000000000L

LARGE_INTEGER getFILETIMEoffset()
{
    SYSTEMTIME s;
    FILETIME f;
    LARGE_INTEGER t;

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}

int clock_gettime(struct timeval *tv)
{
    LARGE_INTEGER t;
    FILETIME f;
    double microseconds;
    static LARGE_INTEGER offset;
    static double frequencyToMicroseconds;
    static int initialized = 0;
    static BOOL usePerformanceCounter = 0;

    if (!initialized)
    {
        LARGE_INTEGER performanceFrequency;
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter)
        {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.;
        }
        else
        {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.;
        }
    }
    if (usePerformanceCounter)
        QueryPerformanceCounter(&t);
    else
    {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }
    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = microseconds;
    tv->tv_sec = t.QuadPart / 1000000;
    tv->tv_usec = t.QuadPart % 1000000;
    return (0);
}
#endif

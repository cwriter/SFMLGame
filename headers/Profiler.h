#pragma once
#include "stdafx.h"
#include <inttypes.h>
#include <stdint.h>
#ifdef _DEBUG
#ifdef _SFGPROFILE
#define SFG_PROFILE_START SFG::Profiler SFG_PROF_p; SFG_PROF_p.profile_start();
#define SFG_PROFILE_MID(x) SFG_PROF_p.profile_mid(x);
#define SFG_PROFILE_END(x) SFG_PROF_p.profile_end(x);
#else
#define SFG_PROFILE_START
#define SFG_PROFILE_MID(x)
#define SFG_PROFILE_END(x)
#endif
#else
#define SFG_PROFILE_START
#define SFG_PROFILE_MID(x)
#define SFG_PROFILE_END(x)
#endif
namespace SFG
{
///<summary>
///Class that provides profiling options. Just for debugging.
///</summary>
class Profiler
{
public:

    void profile_start()
    {
#ifdef _WIN32
        fopen_s(&f, "SFGProfile.txt", "ab");
#else
        f = fopen("SFGProfile.txt", "ab");
#endif // _WIN32
        assert(f != NULL);
        m_clock.restart();
    }

    void profile_mid(const char* dat)
    {
        fprintf(f,"[PROFILER] Inside %s: %" PRId64 " microseconds elapsed.\n", dat, m_clock.getElapsedTime().asMicroseconds());
    }

    void profile_end(const char* dat)
    {
        fprintf(f,"[PROFILER] Calling %s took %" PRId64 " microseconds.\n", dat, m_clock.getElapsedTime().asMicroseconds());
        fclose(f);
    }


private:
    FILE* f;
    sf::Clock m_clock;
};

};

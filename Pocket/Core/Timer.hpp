#pragma once
#ifdef WIN32
#include <windows.h>
#else

#endif



namespace Pocket {

	class Timer
	{
	public:
		Timer();
		~Timer();

		void Begin();
		double End();

	private:
#ifdef WIN32
		LARGE_INTEGER startTime;
		LARGE_INTEGER endTime;
#else
        //clock_t t;
        
        
        struct timer_timeval
        {
#if defined(IPHONE) || defined(ANDROID)
        
            long    tv_sec;        // seconds
            long    tv_usec;    // microSeconds
#else
            long		tv_sec;		/* seconds */
            int	tv_usec;	/* and microseconds */
#endif
        };
        
        timer_timeval t;
        
        
        
         
#endif
	};
}

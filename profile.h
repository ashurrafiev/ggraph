#ifndef _PROFILE_H_
#define _PROFILE_H_

#include <sys/time.h>
#include <iostream>

class Profile {
private:
	struct timeval _start;
	
public:
	bool enabled;
		
	Profile() : enabled(true) {}
	Profile(bool _enabled) : enabled(_enabled) {}
	
	void begin() {
		gettimeofday(&_start, NULL);
	}
	
	double end() {
		struct timeval t, diff;
		gettimeofday(&t, NULL);
		timersub(&t, &_start, &diff);
		return (double) diff.tv_sec + (double) diff.tv_usec / 1000000.0;
	}

	void print(double time, const char* s = "finished") {
		using namespace std;
		if(enabled) {
			cout << " (" << s << " in " << time << "s)";
		}
	}

	void print(const char* s = "finished") {
		print(end(), s);
	}
	
};

#endif

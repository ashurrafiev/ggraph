#ifndef _RANDOM_H_
#define _RANDOM_H_

#include <stdint.h>
#include <assert.h>
#include <sys/time.h>

class Random {
private:
	static const int64_t multiplier = 0x5DEECE66DL;
	static const int64_t addend = 0xBL;
	static const int64_t mask = (1L << 48) - 1;

	uint64_t initialScramble(uint64_t seed) {
		return (seed ^ multiplier) & mask;
	}
	
	uint64_t _seed;
	
	int32_t next(int32_t bits) {
		_seed = (_seed * multiplier + addend) & mask;
		return (int32_t)(_seed >> (48 - bits));
	}
	
public:
	Random() {
		struct timeval t;
		gettimeofday(&t, NULL);
		setSeed((uint64_t)t.tv_sec + (uint64_t)t.tv_usec / 1000);
	}
	
	Random(uint64_t seed) : _seed(initialScramble(seed)) {}
	
	void setSeed(uint64_t seed) {
		_seed = initialScramble(seed);
	}
	
	int32_t nextInt(int32_t bound) {
		assert(bound >0);

		int32_t r = next(31);
		int m = bound - 1;
		if((bound & m)==0)
			r = (int32_t)((bound * (int64_t)r) >> 31);
		else {
			for(int32_t u = r; (u-(r = u%bound) + m)<0; u = next(31));
		}
		return r;
	}
	
	int64_t nextLong() {
		return ((int64_t)(next(32)) << 32) + next(32);
	}
	
	int8_t nextBool() {
		return next(1) != 0;
	}
	
	float nextFloat() {
		return next(24) / ((float)(1 << 24));
	}
	
	double nextDouble() {
		return (((int64_t)(next(26)) << 27) + next(27)) / (double)(1L << 53);
    }
	
	int32_t nextInt(int32_t min, int32_t max) {
		return nextInt(max-min+1)+min;
	}
	
	float nextFloat(float min, float max) {
		return nextFloat()*(max-min)+min;
	}

	double nextDouble(double min, double max) {
		return nextDouble()*(max-min)+min;
	}
	
};

#endif

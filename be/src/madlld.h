//
//  madlld.h
//  soul-sifter
//
//  Created by Robby Neale on 1/5/14.
//
//

#ifndef soul_sifter_madlld_h
#define soul_sifter_madlld_h

#include <boost/function.hpp>

// TODO: replace with std when upgraded to C++11
//typedef void (*ProcessSampleCallback)(const float*, int);
typedef boost::function<void (const float*, int)> ProcessSampleCallback;

int detectBpm(const char* filepath, ProcessSampleCallback processSampleCallback);

#endif

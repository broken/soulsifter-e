//
//  DTVectorUtil.h
//  soul-sifter
//
//  Created by Robby Neale on 2/22/13.
//
//  TODO this could use a rename since it has set utilities as well now

#ifndef soul_sifter_DTVectorUtil_h
#define soul_sifter_DTVectorUtil_h

#include <iostream>
#include <sstream>
#include <set>
#include <vector>

using namespace std;

namespace dogatech {

    template <typename T>
    static bool appendUniqueVector(const vector<T>& src, vector<T>* dest) {
        bool updated = false;
        for (typename vector<T>::const_iterator it = src.begin(); it != src.end(); ++it) {
            if (std::find(dest->begin(), dest->end(), *it) == dest->end()) {
                updated = true;
                dest->push_back(*it);
            }
        }
        return updated;
    }
  
    template <typename T>
    static bool containsVector(const vector<T>& src, const vector<T>& dest) {
        for (typename vector<T>::const_iterator it = src.begin(); it != src.end(); ++it) {
            if (std::find(dest.begin(), dest.end(), *it) == dest.end()) {
                return false;
            }
        }
        return true;
    }
  
    template <typename T>
    static bool containsSet(const set<T>& src, const set<T>& dest) {
        for (typename set<T>::const_iterator it = src.begin(); it != src.end(); ++it) {
            if (std::find(dest.begin(), dest.end(), *it) == dest.end()) {
                return false;
            }
        }
        return true;
    }
  
    template <typename T>
    static bool equivalentVectors(const vector<T>& l, const vector<T>& r) {
        if (l.size() != r.size()) return false;
        // TODO don't i just need to check one if they're the same size?
        return containsVector(l, r) && containsVector(r, l);
    }
  
    template <typename T>
    static bool equivalentSets(const set<T>& l, const set<T>& r) {
        if (l.size() != r.size()) return false;
        return containsSet(l, r);
    }
    
    template <typename T>
    static void deleteVectorPointers(vector<T>* v) {
        while (!v->empty()) delete v->back(), v->pop_back();
    }
  
    template <typename T>
    static const string setToCsv(const set<T>& s) {
        if (s.size() == 0) {
            std::cerr << "Attempting to convert an empty set into a csv." << std::endl;
            return "NULL";
        }
        ostringstream ss;
        typename set<T>::iterator last = s.end();
        --last;
        std::copy(s.begin(), last, ostream_iterator<string>(ss, ","));
        ss << *last;
        return ss.str();
    }

}

#endif

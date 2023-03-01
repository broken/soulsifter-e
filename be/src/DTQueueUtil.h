//
//  DTQueueUtil.h
//  soul-sifter
//
//  Created by Robby Neale on 8/10/13.
//
//  Util similar to the vector one.

#ifndef soul_sifter_DTQueueUtil_h
#define soul_sifter_DTQueueUtil_h

#include <queue>

namespace dogatech {
  
  template <typename T>
  static void deleteQueuePointers(queue<T>* q) {
    while (!q->empty()) delete q->front(), q->pop();
  }
  
}

#endif

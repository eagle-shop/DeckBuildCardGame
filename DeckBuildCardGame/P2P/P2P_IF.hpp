#ifndef P2P_IF_hpp
#define P2P_IF_hpp

#include "EventCtoM.hpp"

using namespace std;

void P2P_sendData(void* data, int size);
void P2P_createData(EVENT_CtoM *outData, EVENT_KIND eventKind, Phase phase, const string *area, const string *name);

#endif /* P2P_IF_hpp */

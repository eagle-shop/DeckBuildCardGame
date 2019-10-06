#ifndef P2P_IF_hpp
#define P2P_IF_hpp

#include "EventCtoM.hpp"

void P2P_sendData(void* data, int size);
void P2P_createData(EVENT_CtoM *outData, EVENT_KIND eventKind, Phase phase, const std::string *area, const std::string *name);

#endif /* P2P_IF_hpp */

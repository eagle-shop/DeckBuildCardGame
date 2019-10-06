#include "P2P_IF.hpp"
#import "P2P.h"

using namespace std;

void P2P_sendData(void* data, int size){
    [P2P sendData:([NSData dataWithBytes:data length:size])];
}

void P2P_createData(EVENT_CtoM *outData, EVENT_KIND eventKind, Phase phase, const string *area, const string *name){
    outData->eventKind = eventKind;
    outData->receiveData.phase = phase;
    outData->eventKind = EVENT_P2P_RECEIVEDATA;
    outData->receiveData.phase = PHASE_UNKNOWN;
    memcpy(&(outData->receiveData.areaName), area->c_str(), area->size());
    memcpy(&(outData->receiveData.cardName), name->c_str(), name->size());
}

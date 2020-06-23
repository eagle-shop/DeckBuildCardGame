#import "ModelBridgeObjC.h"

#include <stdexcept>
#include "Model.hpp"

@implementation Model {
    DeckBuildCardGameModel::Model *m_model;
}

-(id)init {
    self = [super init];
    m_model = new DeckBuildCardGameModel::Model();
    if(m_model == nullptr) {
        throw std::runtime_error("Can't generate a Model");
    }
    return self;
}

-(void)dealloc {
    delete m_model;
}

-(void)action {
    m_model->action();
}

-(void)getSetsData : (NSMutableArray*) out_data {
    auto set = m_model->getSetsData();
    for(const auto& e : set) {
        NSString *name = [NSString stringWithUTF8String: e.name.c_str()];
        [out_data addObject: name];
    }
}

-(bool)createSet : (NSString*) name {
    return m_model->createSet(name.UTF8String);
}

@end

//
//  RRGNotification.cpp
//  RRGNotification
//
//  Created by 山本政徳 on 2015/11/02.
//
//

#include "RRGNotification.h"

using namespace std;
USING_NS_CC;

RRGNotification::RRGNotification()
:_name(""),
_sender(nullptr)
{
    
}

RRGNotification::~RRGNotification()
{
    CC_SAFE_RELEASE_NULL(_sender);
    _vec2Map.clear();
    _sizeMap.clear();
    _rectMap.clear();
    _valueMap.clear();
    _objectMap.clear();
}

RRGNotification* RRGNotification::create(const string& name, Ref* sender)
{
    RRGNotification *ref = new (nothrow) RRGNotification();
    
    if (ref && ref->init(name, sender)) {
        ref->autorelease();
        return ref;
    } else {
        CC_SAFE_DELETE(ref);
        return nullptr;
    }
}

bool RRGNotification::init(const string& name, Ref* sender)
{
    _name = name;
    _sender = sender;
    CC_SAFE_RETAIN(_sender);
    return true;
}
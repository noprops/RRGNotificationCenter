//
//  RRGNotificationCenter.cpp
//  RRGNotification
//
//  Created by 山本政徳 on 2015/11/02.
//
//

#include "RRGNotificationCenter.h"

using namespace std;
USING_NS_CC;

#pragma mark - notification

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

#pragma mark - notification center

namespace {
    RRGNotificationCenter* _sharedInstance = nullptr;
}

RRGNotificationCenter::RRGNotificationCenter()
{
    
}

RRGNotificationCenter::~RRGNotificationCenter()
{
    _notificationMap.clear();
    _noSenderMap.clear();
}

RRGNotificationCenter* RRGNotificationCenter::getInstance()
{
    if (! _sharedInstance) {
        _sharedInstance = new (nothrow) RRGNotificationCenter();
    }
    
    return _sharedInstance;
}

void RRGNotificationCenter::destroyInstance()
{
    CC_SAFE_DELETE(_sharedInstance);
}

void RRGNotificationCenter::addObserver(Ref* observer,
                                        const string& name,
                                        Ref* sender,
                                        const RRGNotificationCallback& callback)
{
    // if sender is null
    if (sender == nullptr) {
        if (_noSenderMap.find(name) == _noSenderMap.end()) {
            ObserverToCallbackMap observerToCallbackMap;
            _noSenderMap.insert(make_pair(name, observerToCallbackMap));
        }
        
        ObserverToCallbackMap& observerToCallbackMap = _noSenderMap.at(name);
        
        observerToCallbackMap.insert(make_pair(observer, callback));
        return;
    }
    
    // if sender is not null
    if (_notificationMap.find(sender) == _notificationMap.end()) {
        NameKeyMap nameKeyMap;
        _notificationMap.insert(make_pair(sender, nameKeyMap));
    }
    
    NameKeyMap& nameKeyMap = _notificationMap.at(sender);
    
    if (nameKeyMap.find(name) == nameKeyMap.end()) {
        ObserverToCallbackMap observerToCallbackMap;
        nameKeyMap.insert(make_pair(name, observerToCallbackMap));
    }
    
    ObserverToCallbackMap& observerToCallbackMap = nameKeyMap.at(name);
    
    observerToCallbackMap.insert(make_pair(observer, callback));
}
void RRGNotificationCenter::removeObserver(Ref* observer,
                                           const string& name,
                                           Ref* sender)
{
    // no sender map
    
    if (sender == nullptr) {
        if (name.empty()) {
            for (auto it = _noSenderMap.begin();
                 it != _noSenderMap.end();
                 ++it)
            {
                it->second.erase(observer);
            }
        } else {
            auto it = _noSenderMap.find(name);
            if (it != _noSenderMap.end()) {
                it->second.erase(observer);
            }
        }
    }
    
    // notification map
    
    auto funcEraseObserberFromNameKeyMap = [observer,name](NameKeyMap& nameKeyMap){
        if (name.empty()) {
            for (auto it = nameKeyMap.begin();
                 it != nameKeyMap.end();
                 ++it)
            {
                it->second.erase(observer);
            }
        } else {
            auto it = nameKeyMap.find(name);
            if (it != nameKeyMap.end()) {
                it->second.erase(observer);
            }
        }
    };
    
    if (sender == nullptr) {
        for (auto it = _notificationMap.begin();
             it != _notificationMap.end();
             ++it)
        {
            funcEraseObserberFromNameKeyMap(it->second);
        }
    } else {
        auto it = _notificationMap.find(sender);
        if (it != _notificationMap.end()) {
            funcEraseObserberFromNameKeyMap(it->second);
        }
    }
    
    // KVO map
    
    if (sender == nullptr) {
        for (auto it = _KVOMap.begin();
             it != _KVOMap.end();
             ++it)
        {
            funcEraseObserberFromNameKeyMap(it->second);
        }
    } else {
        auto it = _KVOMap.find(sender);
        if (it != _KVOMap.end()) {
            funcEraseObserberFromNameKeyMap(it->second);
        }
    }
}
void RRGNotificationCenter::postNotification(RRGNotification* notification)
{
    {
        auto it1 = _noSenderMap.find(notification->getName());
        if (it1 != _noSenderMap.end()) {
            ObserverToCallbackMap& observerToCallbackMap = it1->second;
            for (auto it2 = observerToCallbackMap.begin();
                 it2 != observerToCallbackMap.end();
                 ++it2)
            {
                it2->second(notification);
            }
        }
    }
    
    auto it1 = _notificationMap.find(notification->getSender());
    if (it1 != _notificationMap.end()) {
        NameKeyMap& nameKeyMap = it1->second;
        auto it2 = nameKeyMap.find(notification->getName());
        if (it2 != nameKeyMap.end()) {
            ObserverToCallbackMap& observerToCallbackMap = it2->second;
            for (auto it3 = observerToCallbackMap.begin();
                 it3 != observerToCallbackMap.end();
                 ++it3)
            {
                it3->second(notification);
            }
        }
    }
}
void RRGNotificationCenter::postNotification(const string& name,
                                             Ref* sender)
{
    RRGNotification* note = RRGNotification::create(name, sender);
    postNotification(note);
}
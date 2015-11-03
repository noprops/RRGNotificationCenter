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
                                        const std::string& name,
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
                                           const std::string& name,
                                           Ref* sender)
{
    if (sender == nullptr) {
        for (auto it = _noSenderMap.begin();
             it != _noSenderMap.end();
             ++it)
        {
            if (name.empty() || it->first == name) {
                it->second.erase(observer);
            }
        }
    }
    
    for (auto it1 = _notificationMap.begin();
         it1 != _notificationMap.end();
         ++it1)
    {
        if (sender == nullptr || it1->first == sender) {
            NameKeyMap& nameKeyMap = it1->second;
            
            for (auto it2 = nameKeyMap.begin();
                 it2 != nameKeyMap.end();
                 ++it2)
            {
                if (name.empty() || it2->first == name) {
                    it2->second.erase(observer);
                }
            }
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
void RRGNotificationCenter::postNotification(const std::string& name,
                                             Ref* sender)
{
    RRGNotification* note = RRGNotification::create(name, sender);
    postNotification(note);
}

#pragma mark - KVO

void RRGNotificationCenter::removeKeyValueObserver(Ref* observer,
                                                   const std::string& key,
                                                   Ref* sender)
{
    for (auto it1 = _KVOMap.begin();
         it1 != _KVOMap.end();
         ++it1)
    {
        if (it1->first == sender) {
            NameKeyMap& nameKeyMap = it1->second;
            
            for (auto it2 = nameKeyMap.begin();
                 it2 != nameKeyMap.end();
                 ++it2)
            {
                if (name.empty() || it2->first == name) {
                    it2->second.erase(observer);
                }
            }
        }
    }
}
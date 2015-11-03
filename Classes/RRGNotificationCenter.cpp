//
//  RRGNotificationCenter.cpp
//  RRGNotification
//
//  Created by 山本政徳 on 2015/11/02.
//
//

#include "RRGNotificationCenter.h"
#include "RRGNotification.h"

using namespace std;
USING_NS_CC;

namespace {
    RRGNotificationCenter* _sharedInstance = nullptr;
}

RRGNotificationCenter::RRGNotificationCenter()
{
    
}

RRGNotificationCenter::~RRGNotificationCenter()
{
    _callbackMap.clear();
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
    if (_callbackMap.find(name) == _callbackMap.end()) {
        SenderToMap senderToMap;
        _callbackMap.insert(make_pair(name, senderToMap));
    }
    
    SenderToMap& senderToMap = _callbackMap.at(name);
    
    if (senderToMap.find(sender) == senderToMap.end()) {
        ObserverToCallbackMap observerToCallbackMap;
        senderToMap.insert(make_pair(sender, observerToCallbackMap));
    }
    
    ObserverToCallbackMap& observerToCallbackMap = senderToMap.at(sender);
    
    observerToCallbackMap.insert(make_pair(observer, callback));
}
void RRGNotificationCenter::removeObserver(Ref* observer,
                                           const std::string& name,
                                           Ref* sender)
{
    for (auto it1 = _callbackMap.begin();
         it1 != _callbackMap.end();
         ++it1)
    {
        if (name.empty() || it1->first == name) {
            SenderToMap& senderToMap = it1->second;
            
            for (auto it2 = senderToMap.begin();
                 it2 != senderToMap.end();
                 ++it2)
            {
                if (sender == nullptr || it2->first == sender) {
                    ObserverToCallbackMap& observerToCallbackMap = it2->second;
                    observerToCallbackMap.erase(observer);
                }
            }
        }
    }
}
void RRGNotificationCenter::postNotification(RRGNotification* notification)
{
    auto it1 = _callbackMap.find(notification->getName());
    if (it1 != _callbackMap.end()) {
        SenderToMap& senderToMap = it1->second;
        auto it2 = senderToMap.find(notification->getSender());
        if (it2 != senderToMap.end()) {
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
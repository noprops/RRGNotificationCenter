//
//  RRGNotificationCenter_Private.h
//  RRGNotification
//
//  Created by 山本政徳 on 2015/11/03.
//
//

#ifndef __RRGNotification__RRGNotificationCenter_Private__
#define __RRGNotification__RRGNotificationCenter_Private__

#include "RRGNotificationCenter.h"

namespace {
    const char* kNotificationOldKey = "__old__";
    const char* kNotificationNewKey = "__new__";
}

template <typename T>
inline void RRGNotificationCenter::addKeyValueObserver(Ref* observer,
                                                       const std::string& key,
                                                       Ref* sender,
                                                       const std::function<void(T oldVal, T newVal)>& callback)
{
    if (_KVOMap.find(sender) == _KVOMap.end()) {
        NameKeyMap nameKeyMap;
        _KVOMap.insert(std::make_pair(sender, nameKeyMap));
    }
    NameKeyMap& nameKeyMap = _KVOMap.at(sender);
    if (nameKeyMap.find(key) == nameKeyMap.end()) {
        ObserverToCallbackMap observerToCallbackMap;
        nameKeyMap.insert(std::make_pair(key, observerToCallbackMap));
    }
    ObserverToCallbackMap& observerToCallbackMap = nameKeyMap.at(key);
    auto callback2 = [callback](RRGNotification* note){
        T oldVal = note->getValue<T>(kNotificationOldKey);
        T newVal = note->getValue<T>(kNotificationNewKey);
        callback(oldVal, newVal);
    };
    observerToCallbackMap.insert(std::make_pair(observer, callback2));
}

template <typename T>
inline void RRGNotificationCenter::postDidChangeValueNotification(const std::string& key,
                                                                  cocos2d::Ref* sender,
                                                                  T oldVal,
                                                                  T newVal)
{
    RRGNotification* note = RRGNotification::create(key, sender);
    note->addValue(oldVal, kNotificationOldKey);
    note->addValue(newVal, kNotificationNewKey);
    postNotification(note);
}

#endif /* defined(__RRGNotification__RRGNotificationCenter_Private__) */
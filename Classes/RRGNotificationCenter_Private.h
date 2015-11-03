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

template <typename T>
inline void RRGNotificationCenter::addObserverForKey(Ref* observer,
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
    [](RRGNotification* note){
        note->getva
    }
    observerToCallbackMap.insert(std::make_pair(observer, <#_T2 &&__t2#>))
}

#endif /* defined(__RRGNotification__RRGNotificationCenter_Private__) */
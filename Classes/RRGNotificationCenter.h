//
//  RRGNotificationCenter.h
//  RRGNotification
//
//  Created by 山本政徳 on 2015/11/02.
//
//

#ifndef __RRGNotification__RRGNotificationCenter__
#define __RRGNotification__RRGNotificationCenter__

#include "cocos2d.h"

#define sharedNotificationCenter RRGNotificationCenter::getInstance()

class RRGNotification;
class RRGNotificationCenter : public cocos2d::Ref
{
public:
    typedef std::function<void (RRGNotification*)> RRGNotificationCallback;
private:
    typedef std::unordered_map<Ref*, RRGNotificationCallback> ObserverToCallbackMap;
    typedef std::unordered_map<Ref*, ObserverToCallbackMap> SenderToMap;
    
    std::unordered_map<std::string, SenderToMap> _callbackMap;
public:
    RRGNotificationCenter();
    ~RRGNotificationCenter();
    
    static RRGNotificationCenter* getInstance();
    static void destroyInstance();
    
    void addObserver(Ref* observer,
                     const std::string& name,
                     Ref* sender,
                     const RRGNotificationCallback& callback);
    void removeObserver(Ref* observer,
                        const std::string& name = "",
                        Ref* sender = nullptr);
    void postNotification(RRGNotification* notification);
    void postNotification(const std::string& name,
                          Ref* sender);
};

#endif /* defined(__RRGNotification__RRGNotificationCenter__) */

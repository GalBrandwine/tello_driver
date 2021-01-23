/**
 * Observer Design Pattern
 *
 * Intent: Lets you define a subscription mechanism to notify multiple objects
 * about any events that happen to the object they're observing.
 *
 * Note that there's a lot of different terms with similar meaning associated
 * with this pattern. Just remember that the Subject is also called the
 * Publisher and the Observer is often called the Subscriber and vice versa.
 * Also the verbs "observe", "listen" or "track" usually mean the same thing.
 */
#pragma once
#include "DataMgr.hpp"
#include "IObserver.hpp"
class IDataMgrSubject
{
public:
  virtual ~IDataMgrSubject(){};
  virtual void Attach(const OBSERVERS observer_type, IObserver *observer) = 0;
  virtual void Notify(const OBSERVERS observer_type) = 0;
};
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
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "utils/telemetry_data/TelemetryData.hpp"
class IPositionVelocityObserver : public IObserver
{
public:
  virtual ~IPositionVelocityObserver(){};
  virtual void Update(const tello_protocol::PoseVelData &pos_vel) = 0;
  
  /**
   * @brief Empty implementation 
   * 
   * @param message_from_subject 
   */
  void Update(const std::vector<unsigned char> &message_from_subject) override{};
};
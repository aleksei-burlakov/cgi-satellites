#include "timeslots.hpp"
#include <math.h>

namespace ts
{
  void PackTimestamps( TimeMap& timeMap, const ns_time::TimeSlots& timeSlots )
  {
    timeMap.clear();
    for ( const auto& timeSlot : timeSlots )
    {
      auto timeStamp = timeMap.find( timeSlot.m_begin );
      if ( timeStamp == timeMap.end() )
        timeMap.insert( { timeSlot.m_begin, 1 } );
      else
        timeStamp->second++;

      timeStamp = timeMap.find( timeSlot.m_end );
      if ( timeStamp == timeMap.end() )
        timeMap.insert( { timeSlot.m_end, -1 } );
      else
        timeStamp->second--;
    }
  }

  void ComputeMaximumAvailableSatellites( unsigned& maxSatellites, TimeMap& timeMap )
  {
    int _maxSatellites = 0;
    int minSatellites = 0; // check if there is a negative number of satellites
    int cntSatellites = 0; // A number of satellites at a timeStamp
    for ( auto& timeStamp : timeMap )
    {
      cntSatellites += timeStamp.second;
      timeStamp.second = cntSatellites;
      if ( _maxSatellites < cntSatellites )
        _maxSatellites = cntSatellites;
      if ( cntSatellites < minSatellites )
        minSatellites = cntSatellites;
    }
    /* It may happen that in [t1,t2] t1 > t2
     * this may lead to the case that there are 
     * negative number of satellites above the horizont. 
     * The loop below fixes the problem. */
    if ( minSatellites < 0 ) {
      minSatellites = std::abs( minSatellites );
      _maxSatellites += minSatellites;
      for ( auto& timeStamp : timeMap )
      {
        timeStamp.second += minSatellites;
      }
    }
    maxSatellites = static_cast<unsigned>(_maxSatellites);
  }

  void ComputeTimeslotsWithMostSatellites( ns_time::TimeSlots& timeSlotsWithMostSatellites
    , unsigned& maxSatellites, const ns_time::TimeSlots& timeSlots )
  {
    TimeMap timeMap;
    PackTimestamps( timeMap, timeSlots );
    ComputeMaximumAvailableSatellites( maxSatellites, timeMap );

    ns_time::Time begin; // When a slot starts
    bool isRecording = false; // Slot is open?
    for ( const auto& timeStamp : timeMap )
    {
      if ( !isRecording && timeStamp.second == maxSatellites ) {
        isRecording = true;
        begin = ns_time::Time( timeStamp.first );
      }
      if ( isRecording && static_cast<unsigned>(timeStamp.second) < maxSatellites ) {
        ns_time::Time end = ns_time::Time( timeStamp.first );
        timeSlotsWithMostSatellites.push_back( { begin , end } );
        isRecording = false;
      }
    }
    // The case when a [t1,t2), t1 > t2
    if ( isRecording ) {
      for ( const auto& timeStamp : timeMap )
      {
        if ( isRecording && static_cast<unsigned>(timeStamp.second) < maxSatellites ) {
          ns_time::Time end = ns_time::Time( timeStamp.first );
          timeSlotsWithMostSatellites.push_back( { begin , end } );
          isRecording = false;
          break;
        }
      }
    }
  }
}
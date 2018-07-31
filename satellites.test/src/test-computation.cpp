#include "main.hpp"
#include "misc.hpp"
#include "timeslots.hpp"

using namespace ns_time;
using namespace ts;

TEST_F( TestComputation, PackTimestamps )
{
  TimeMap timeMap;
  TimeSlots timeSlots;
  timeSlots.push_back( { 0, 1 } ); // 1st sat. appears at 0 disappear at 1
  timeSlots.push_back( { 0, 2 } ); // 2ns sat. appears at 0 disappear at 2
  timeSlots.push_back( { 0, 2 } ); // 3rd sat. appears at 0 disappear at 2
  timeSlots.push_back( { 3, 4 } ); // 4th sat. appears at 3 disappear at 4
  PackTimestamps( timeMap, timeSlots );
  TimeMap timeMapExpected;
  timeMapExpected.insert( { 0, 3 } );  // at 0 three sat. appear
  timeMapExpected.insert( { 1, -1 } ); // at 1 one disappears
  timeMapExpected.insert( { 2, -2 } ); // at 2 two disappear
  timeMapExpected.insert( { 3, 1 } );  // at 3 one appear
  timeMapExpected.insert( { 4, -1 } ); // at 4 one disappear
  ASSERT_THAT( timeMapExpected, ::testing::ContainerEq( timeMap ) );
}

TEST_F( TestComputation, ComputeMaximumAvailableSatellites )
{
  unsigned maxSatellites = 0;
  TimeMap timeMap;
  /* It is possible that a satellite appeared afternoon
   * and disappeared before noon [t1,t2), t1 > t2
   * (e.g. it is possible when another time zone is used). */
  timeMap.insert( { 0, 0 } );
  timeMap.insert( { 1, -1 } ); // at 1ms one sat. disappears
  timeMap.insert( { 2, -1 } );
  timeMap.insert( { 3, 1 } );
  timeMap.insert( { 4, -1 } );
  timeMap.insert( { 5, -1 } );
  timeMap.insert( { 6, 1 } );
  timeMap.insert( { 7, 1 } );
  timeMap.insert( { 8, 1 } );
  ComputeMaximumAvailableSatellites( maxSatellites, timeMap );
  TimeMap timeMapExpected;
  timeMapExpected.insert( { 0, 3 } );
  timeMapExpected.insert( { 1, 2 } ); // at 1ms there are two sats.
  timeMapExpected.insert( { 2, 1 } );
  timeMapExpected.insert( { 3, 2 } );
  timeMapExpected.insert( { 4, 1 } );
  timeMapExpected.insert( { 5, 0 } );
  timeMapExpected.insert( { 6, 1 } );
  timeMapExpected.insert( { 7, 2 } );
  timeMapExpected.insert( { 8, 3 } );
  ASSERT_EQ( maxSatellites, 3 );
  ASSERT_THAT( timeMapExpected, ::testing::ContainerEq( timeMap ) );
}

TEST_F( TestComputation, ComputeTimeslotsWithMostSatellites )
{
  TimeSlots timeSlotsWithMostSatellites;
  unsigned maxSatellites;
  TimeSlots timeSlots;
  /* It is possible that a satellite appeared afternoon
   * and disappeared before noon [t1,t2), t1 > t2
   * (e.g. it is possible when another time zone is used). */
  timeSlots.push_back( { 4, 5 } );
  timeSlots.push_back( { 9, 2 } );
  timeSlots.push_back( { 6, 3 } );
  timeSlots.push_back( { 7, 6 } );

  ComputeTimeslotsWithMostSatellites( timeSlotsWithMostSatellites, maxSatellites, timeSlots );
  // There are 2 satellites from 9 of one day to 2 of the ext day.
  ASSERT_EQ( maxSatellites, 2 );
  ASSERT_THAT( TimeSlots( { { 9, 2 } } )
    , ::testing::ContainerEq( timeSlotsWithMostSatellites ) );
}
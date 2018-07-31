#include "misc.hpp"
#include "timeslots.hpp"
#include <iostream>

int main( int argc, char* argv[] )
{
  try {
    // Sanity checks
    if( 1 == argc)
      throw  "Please provide a path to the data file.";
    else if( 2 < argc )
      throw  "Wrong format of the command.";

    // Parse the file into timeSlots
    std::string fileName = std::string( argv[1] );
    ns_time::TimeSlots timeSlots;
    ns_time::ParseFile( timeSlots , fileName );
    
    // Get timeslots when the maximum number of satellites are visible
    ns_time::TimeSlots timeSlotsWithMostSatellites;
    unsigned maxSatellites = 0;
    ts::ComputeTimeslotsWithMostSatellites( timeSlotsWithMostSatellites, maxSatellites, timeSlots );

    // Print the result
    for ( const auto& ts : timeSlotsWithMostSatellites )
    {
      std::cout << ns_time::toString( ts.m_begin )
        << "-" << ns_time::toString( ts.m_end ) << ";" << maxSatellites << std::endl;
    }
  }
  catch ( const char* message ) {
    std::cout << message << std::endl;
  }
  return 0;
}
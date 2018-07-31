#define _CRT_SECURE_NO_WARNINGS // msvc complains about sscanf
#include "misc.hpp"
#include <stdarg.h>
#include <fstream>
#include <string.h>

void error( const char *fileName, int lineNumber, const char *format, ... )
{
  char* c = (char*)&fileName[strlen( fileName ) - 1];
  for ( ; c > fileName; c-- )
  {
    if (( '\\' == *c ) || ('/' == *c ))
      break;
  }
  fileName = c + 1;

  char buff[128];
  static char message[512]; // We may throw only static strings

  snprintf( buff, sizeof( buff ), "ERROR: %s: line %d: %s", fileName, lineNumber, format );
  buff[sizeof( buff ) - 1] = '\0';
  
  va_list ap;
  va_start( ap, format );
  vsnprintf( message, sizeof( message ), buff, ap );
  va_end( ap );
  throw (const char*)message;
}

namespace ns_time
{
  std::string toString( Time milliseconds )
  {
    // MILLISECONDS_IN_DAY + 1 is the maximum possible time when a satellite may disappear
    ASSERT( milliseconds < MILLISECONDS_IN_DAY + 1, "Wrong input." );
    int hours = milliseconds / MILLISECONDS_IN_HOUR;
    milliseconds %= MILLISECONDS_IN_HOUR;
    int minutes = milliseconds / MILLISECONDS_IN_MINUTE;
    milliseconds %= MILLISECONDS_IN_MINUTE;
    int seconds = milliseconds / MILLISECONDS_IN_SECOND;
    milliseconds %= MILLISECONDS_IN_SECOND;

    char buffer[13] = { 0 };
    snprintf( buffer, sizeof( buffer ), "%.2d:%.2d:%.2d.%.3d", hours, minutes, seconds, milliseconds );
    return std::string( buffer );
  }

  CTimeSlot ParseLine( const std::string& line, int lineNumber )
  {
    int hourBegin( 0 ), minuteBegin( 0 ), secondBegin( 0 ), millisecondBegin( 0 )
      , hourEnd( 0 ), minuteEnd( 0 ), secondEnd( 0 ), millisecondEnd( 0 );
    int r = sscanf( line.c_str(), "%d:%d:%d.%d,%d:%d:%d.%d"
      , &hourBegin, &minuteBegin, &secondBegin, &millisecondBegin
      , &hourEnd, &minuteEnd, &secondEnd, &millisecondEnd );
    // Validation
    ASSERT( 8 == r, "Wrong time format in Ln %d \"%s\"", lineNumber, line.c_str() );
    ASSERT( (hourBegin < 24) && (hourEnd < 24), "hours should be 0 - 23 in Ln %d \"%s\""
      , lineNumber, line.c_str() );
    ASSERT( (minuteBegin < 60) && (minuteEnd < 60), "minutes should be 0 - 59 in Ln %d \"%s\""
      , lineNumber, line.c_str() );
    ASSERT( (secondBegin < 60) && (secondEnd < 60), "seconds should be 0 - 59 in Ln %d \"%s\""
      , lineNumber, line.c_str() );
    ASSERT( (millisecondBegin <= 1000) && (millisecondEnd <= 1000)
      , "milliseconds should be 0 - 999 in Ln %d \"%s\""
      , lineNumber, line.c_str() );
    Time begin = toTime( hourBegin, minuteBegin, secondBegin, millisecondBegin );
    Time end = toTime( hourEnd, minuteEnd, secondEnd, millisecondEnd );
    return{ begin, end + 1 /* when a satellite disappears */ };
  }

  void ParseFile( TimeSlots& timeSlots, const std::string& fileName )
  {
    std::fstream myfile( fileName, std::ios::in );
    myfile.seekg( 0, std::ios::beg );
    int lineNumber = 1;
    ASSERT( myfile.is_open(), "Unable to open the file %s, ", fileName.c_str() );
    std::string line;
    while ( std::getline( myfile, line ) )
    {
      timeSlots.push_back( ParseLine( line, lineNumber++ ) );
    }
    myfile.close();
  }
}
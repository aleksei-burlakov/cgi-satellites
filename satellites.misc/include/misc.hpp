#pragma once
#include <string>
#include <list>

void error( const char *fileName, int lineNumber, const char *format, ... );
#define ASSERT(condition, fmt, ...)   if( !(condition) ){ error(__FILE__, __LINE__, fmt, ## __VA_ARGS__); }

namespace ns_time
{
  const unsigned MILLISECONDS_IN_SECOND = 1000;
  const unsigned MILLISECONDS_IN_MINUTE = 60 * MILLISECONDS_IN_SECOND;
  const unsigned MILLISECONDS_IN_HOUR = 60 * MILLISECONDS_IN_MINUTE;
  const unsigned MILLISECONDS_IN_DAY = 24 * MILLISECONDS_IN_HOUR;

  typedef unsigned Milliseconds;
  typedef Milliseconds Time; // Milliseconds from the midnight

  struct CTimeSlot
  {
    Time m_begin;
    Time m_end;
    bool operator==( const CTimeSlot& that ) const // for tests
    {
      return (m_begin == that.m_begin) && (m_end == that.m_end);
    }
  };

  typedef std::list<CTimeSlot> TimeSlots;

  // Convert hour,minutes,seconds, and milliseconds into miiseonds from the midnight
  inline Time toTime( unsigned hours, unsigned minutes, unsigned seconds, unsigned milliseconds )
  {
    ASSERT( (hours < 24) && (minutes < 60) && (seconds < 60) && (milliseconds < 1000)
      , "Wrong input." );
    return hours * MILLISECONDS_IN_HOUR
      + minutes * MILLISECONDS_IN_MINUTE
      + seconds * MILLISECONDS_IN_SECOND
      + milliseconds;
  }
  // Convert milisseconds from the midnight into hh:mm:ss:xxx format, where xxx - milliseconds
  std::string toString( Time milliseconds );
  /* Input: 1) lineNumber - optional, does not influence the result
   *        2) line
   * Result: time slot 
   * The line format is CCSDS ASCII Time Code A, but without a date 
   * and in milliseconds precision. */
  CTimeSlot ParseLine( const std::string& line, int lineNumber );
  /* Input: fileName
   * Output timeSlots */
  void ParseFile( TimeSlots& timeSlots, const std::string& fileName );
}
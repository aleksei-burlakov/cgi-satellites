#include "main.hpp"
#include "misc.hpp"

using namespace ns_time;

TEST_F( TestMisc, toTime )
{
  ASSERT_EQ( toTime( 0, 0, 0, 0 ), 0 );
  ASSERT_EQ( toTime( 0, 0, 0, 123 ), 123 );
  ASSERT_EQ( toTime( 3, 25, 45, 678 ), 12345678 );
  // Invalid times
  ASSERT_ANY_THROW( toTime( -1, 0, 0, 0 ) );
  ASSERT_ANY_THROW( toTime( 100, 0, 0, 0 ) );
  ASSERT_ANY_THROW( toTime( 0, -1, 0, 0 ) );
  ASSERT_ANY_THROW( toTime( 0, 100, 0, 0 ) );
}

TEST_F( TestMisc, toString )
{
  ASSERT_STREQ( toString( 0 ).c_str(), "00:00:00.000" );
  ASSERT_STREQ( toString( 123 ).c_str(), "00:00:00.123" );
  ASSERT_STREQ( toString( 12345678 ).c_str(), "03:25:45.678" );
  // Invalid time values
  ASSERT_ANY_THROW( toString( -1 ) );
  ASSERT_ANY_THROW( toString( ns_time::MILLISECONDS_IN_DAY + 1 ) );
}

TEST_F( TestMisc, ParseLine )
{
  CTimeSlot timeSlot = ParseLine( "00:00:00.000,00:00:00.000", 0 );
  ASSERT_EQ( timeSlot.m_begin, 0 );
  ASSERT_EQ( timeSlot.m_end, 0 + 1 );
  timeSlot = ParseLine( "00:00:00.123,03:25:45.678", 0 );
  ASSERT_EQ( timeSlot.m_begin, 123 );
  ASSERT_EQ( timeSlot.m_end, 12345678 + 1 );
  timeSlot = ParseLine( "00:00:00.0,00:00:00.0", 0 );
  ASSERT_EQ( timeSlot.m_begin, 0 );
  ASSERT_EQ( timeSlot.m_end, 0 + 1 );
  // Invalid time formats
  ASSERT_ANY_THROW( ParseLine( "23:61:00.000,00:00:00.000", 0 ) );
  ASSERT_ANY_THROW( ParseLine( "00:00:00,00:00:00.000", 0 ) );
  ASSERT_ANY_THROW( ParseLine( "00:00:00.000;00:00:00.000", 0 ) );
  ASSERT_ANY_THROW( ParseLine( "00:0a:00.000,00:00:00.000", 0 ) );
  ASSERT_ANY_THROW( ParseLine( "00:00:00.000AM,00:00:00.000", 0 ) );
  ASSERT_ANY_THROW( ParseLine( "", 2 ) );
}

TEST_F( TestMisc, ParseFile )
{
  TimeSlots timeSlots;
  // File cannot be read/not exists
  ASSERT_ANY_THROW( ParseFile( timeSlots , "ads%^!") );
}
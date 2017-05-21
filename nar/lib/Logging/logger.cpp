#include "boost/date_time/posix_time/posix_time.hpp" //include all types plus i/o
#include "logger.h"

template <typename T>
std::ostream& nar::Logger::operator<<( const T v ) {
    boost::posix_time::ptime t(boost::posix_time::microsec_clock::local_time());
    _stream << t.time_of_day();
    _stream << v;
    return _stream;
}

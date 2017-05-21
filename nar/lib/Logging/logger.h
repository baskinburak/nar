#ifndef NAR_LOGGER_H
#define NAR_LOGGER_H

#include <string>
#include <iostream>
#include <mutex>
#include <thread>
#include "boost/date_time/posix_time/posix_time.hpp" //include all types plus i/o


namespace nar {

    /**
    * Class which provides the interface and services for logging customization
    *
    * @author: utku
    * @privar: _stream, std::stream, stream to write the log messages.
    * @privar: _, boost::asio::ip::tcp::socket, holds the boost tcp-ip socket in the case of initialization as 'c' (client) socket
    * @tested: no
    * @todo: -
    **/

    class Logger
    {
        public:

            Logger(std::ostream& stream ) : _stream(stream) {}

            template<typename T>
            std::ostream& operator<<( const T& v ) {
                log_mtx.lock();

                boost::posix_time::ptime t(boost::posix_time::microsec_clock::local_time());

                _stream<<"[" << t.date() << " "<<t.time_of_day()<<", "<< std::hash<std::thread::id>()(std::this_thread::get_id()) <<"] " ;
                _stream << v;
                log_mtx.unlock();
                return _stream;
            }

        private:
            std::mutex log_mtx;
            std::ostream& _stream;
    };
}

#endif

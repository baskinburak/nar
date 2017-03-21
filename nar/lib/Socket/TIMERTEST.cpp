#include <boost/asio.hpp>
#include <iostream>
#include <atomic>
#include <thread>
using std::cout;
using std::endl;

void timer_expire(boost::asio::io_service* io_serv, unsigned long usec, std::atomic<bool>* stop_timer, std::atomic<bool>* timer_flag) {
    boost::asio::deadline_timer timer(*io_serv, boost::posix_time::microseconds(usec));
    timer.wait();
    if(*stop_timer) {
        delete stop_timer;
        delete timer_flag;
    } else {
        *timer_flag = true;
    }
}

int main() {
    boost::asio::io_service io_serv;
    std::atomic<bool>* stop_timer = new std::atomic<bool>;
    *stop_timer = false;
    std::atomic<bool>* timer_flag = new std::atomic<bool>;
    *timer_flag = false;
    std::thread thr(timer_expire, &io_serv, 1000000, stop_timer, timer_flag);
    *stop_timer = true;
    stop_timer = new std::atomic<bool>;
    *stop_timer = false;
    thr.join();
    cout << *timer_flag << endl;
    cout << "nbr" << endl;
    return 0;
}

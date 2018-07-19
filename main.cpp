#include <memory>
#include <iostream>
#include <string>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
int main(int argc, char ** argv) {
    struct Context {
        boost::mutex mutex;
        boost::condition_variable cv;
        boost::thread thread;
        std::atomic<bool> should_stop;

        Context() {
            should_stop = false;
            thread = boost::thread([this] () {
                    run();
                });
            boost::this_thread::sleep_for(boost::chrono::seconds(1));
        }

        void run() {
            boost::mutex::scoped_lock lock(mutex);
            boost::posix_time::milliseconds wait_for_ms(1);
            while(!should_stop) {
                std::cout << "wait\n";
                cv.timed_wait(lock, wait_for_ms);
                std::cout << "run\n";
                boost::this_thread::sleep_for(boost::chrono::milliseconds(300));
            }
            std::cout << "done\n";
        }

        ~Context() {
            {
                boost::mutex::scoped_lock lock(mutex);
                should_stop = true;
                cv.notify_one();
                std::cout << "join\n";
            }
            thread.join();
        }

    };

    Context ctx;
}
#ifndef _SharedQueue_H_
#define _SharedQueue_H_

#include "Common.h"
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class SharedQueue{
    static constexpr std::size_t default_max_length = 1000;
    public:
        SharedQueue(){
            max_length = default_max_length;
        };
        ~SharedQueue(){};

    private:
        std::size_t max_length;
        std::queue<T> buffer;
        bool closed = false;
        
        std::mutex _mtx;
        std::condition_variable _cond;

    public:
        void put(T obj){
            std::unique_lock<std::mutex> locker(_mtx);
            _cond.wait(locker, [this](){
                return buffer.size() < max_length;
            });
    
            buffer.push(std::move(obj));
            _cond.notify_one();
        };

        T get(){
            std::unique_lock<std::mutex> locker(_mtx);
            _cond.wait(locker, [this](){
                return !buffer.empty() || closed;
            });

            if(buffer.empty()){
                return NULL;
            }

            T obj = buffer.front();
            buffer.pop();
            _cond.notify_one();
            return obj;
        };
};

#endif
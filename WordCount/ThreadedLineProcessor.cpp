//
//  ThreadedLineProcessor.cpp
//  WordCount
//
//  Created by Mikhail Gorshkov on 27.11.2021.
//  Copyright © 2021 Mikhail Gorshkov. All rights reserved.
//

#include "ThreadedLineProcessor.hpp"
#include "WordCollector.hpp"
#include "LineProcessor.hpp"

#include <iostream>
#include <cassert>

//#define DEBUG_LOG

using namespace std::chrono_literals;

#ifdef __gnu_linux__
static int pin_to_cpu(pthread_t thread_handle, int cpu) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    return pthread_setaffinity_np(thread_handle, sizeof(cpuset), &cpuset);
}
#endif

ThreadedLineProcessor::ThreadedLineProcessor(WordCollector* wordCollector)
    : m_wordCollector(wordCollector)
{
    m_finalize.store(false, std::memory_order_relaxed);
    static const unsigned int processor_count = std::thread::hardware_concurrency();
    for (int i = 0; i < processor_count; ++i) {
        m_threadPool.emplace_back(&ThreadedLineProcessor::threadProc, this);
#ifdef __gnu_linux__
        const int cpu = i % processor_count;
        int res = pin_to_cpu(m_threadPool[i].native_handle(), cpu);
        assert(res == 0);
#endif
    }
}

ThreadedLineProcessor::~ThreadedLineProcessor() {
    finalizePool();
}

#ifdef DEBUG_LOG
int pushes = 0;
int pops = 0;
#endif

void ThreadedLineProcessor::processBuf(const char* buf, std::size_t size) {
    while (checkWatermark()) { // backpressure the producer if the consumer doesn't keep up
        std::this_thread::sleep_for(500ms);
#ifdef DEBUG_LOG
        std::cout << "Backpressure" << std::endl;
#endif
    }
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(std::string(buf, size));
#ifdef DEBUG_LOG
        ++pushes;
#endif
    }

    m_condition.notify_one();
#ifdef DEBUG_LOG
    std::cout << "Notified" << std::endl;
#endif
}

bool ThreadedLineProcessor::checkWatermark() {
    std::lock_guard<std::mutex> lock(m_mutex);
#ifdef DEBUG_LOG
    std::cout << "Queue size=" << m_queue.size() << std::endl;
#endif
    return m_queue.size() >= MAX_QUEUE_SIZE;
}

void ThreadedLineProcessor::threadProc()
{
    try {
        WordCollector collector;
        LineProcessor processor(&collector);
        while (true)
        {
            std::string line;
            {
                std::unique_lock<std::mutex> lock(m_mutex);
#ifdef DEBUG_LOG
                std::cout << "before wait" << std::endl;
#endif
                m_condition.wait(lock, [this](){ return !m_queue.empty() || m_finalize.load(std::memory_order_relaxed); });
#ifdef DEBUG_LOG
                std::cout << "after wait" << std::endl;
                std::cout << m_queue.size() << std::endl;
#endif
                if (m_queue.empty())
                    break;
	   
                line = m_queue.front();
                m_queue.pop();
#ifdef DEBUG_LOG
                ++pops;
#endif
            }
            processor.processBuf(line.c_str(), line.size());
        }
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_wordCollector->merge(collector);
        }
    } catch (...)
    {
        std::cerr << "Exception caught in threadProc" << std::endl;
    }
}

void ThreadedLineProcessor::finalizePool()
{
    m_finalize.store(true, std::memory_order_relaxed);
    m_condition.notify_all();
#ifdef DEBUG_LOG
    std::cout << "Notify all" << std::endl;
#endif
    for (auto& thread : m_threadPool)
        thread.join();
#ifdef DEBUG_LOG
    std::cout << "totally " << pushes << " pushes and " << pops << " pops " << std::endl; 
#endif
}

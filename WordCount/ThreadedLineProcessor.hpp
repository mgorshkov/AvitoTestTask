//
//  ThreadedLineProcessor.hpp
//  WordCount
//
//  Created by Mikhail Gorshkov on 27.11.2021.
//  Copyright © 2021 Mikhail Gorshkov. All rights reserved.
//

#ifndef ThreadedLineProcessor_hpp
#define ThreadedLineProcessor_hpp

#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>
#include <thread>

#include "ILineProcessor.hpp"
#include "WordCollector.hpp"

class ThreadedLineProcessor : public ILineProcessor {
public:
    ThreadedLineProcessor(WordCollector* wordCollector);
    
    ~ThreadedLineProcessor();
    
    void processBuf(const char* buf, std::size_t size) override;
        
private:
    void threadProc();
    bool checkWatermark();
    void finalizePool();
    
    static const size_t MAX_QUEUE_SIZE = 100;

    WordCollector* m_wordCollector;
    
    std::queue<std::string> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condition;
    std::vector<std::thread> m_threadPool;
    std::atomic_bool m_finalize;
};

#endif /* ThreadedLineProcessor_hpp */

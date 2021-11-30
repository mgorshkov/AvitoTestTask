//
//  WordCollector.hpp
//  WordCount
//
//  Created by Mikhail Gorshkov on 26.11.2021.
//  Copyright © 2021 Mikhail Gorshkov. All rights reserved.
//

#ifndef WordCollector_hpp
#define WordCollector_hpp

#include <string>
#include <iostream>
#include <unordered_map>

#include "WordCollector.hpp"

using WordType = std::string;

class WordCollector {
public:
    void collectWord(const WordType& word);
    void merge(const WordCollector& another);
    
private:
    friend std::ostream& operator << (std::ostream& ostream, const WordCollector& collector);
    
    using CounterType = long;
    using WordCounters = std::unordered_map<WordType, CounterType>;
    WordCounters m_wordCounters;
};

#endif /* WordCollector_hpp */

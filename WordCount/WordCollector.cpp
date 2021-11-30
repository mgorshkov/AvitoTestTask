//
//  WordCollector.cpp
//  WordCount
//
//  Created by Mikhail Gorshkov on 26.11.2021.
//  Copyright © 2021 Mikhail Gorshkov. All rights reserved.
//

#include "WordCollector.hpp"

#include <map>
#include <set>

void WordCollector::collectWord(const WordType& word) {
    ++m_wordCounters[word];
}

void WordCollector::merge(const WordCollector& another) {
    for (const auto& counter : another.m_wordCounters) {
        m_wordCounters[counter.first] += counter.second;
    }
}
    
std::ostream& operator << (std::ostream& ostream, const WordCollector& container) {
    using SortedWordCounters = std::map<WordCollector::CounterType, std::set<WordType>, std::greater<WordCollector::CounterType>>;
    
    SortedWordCounters sortedWordCounters;
    for (const auto& counter : container.m_wordCounters) {
        sortedWordCounters[counter.second].insert(counter.first);
    }
    for (const auto& counter : sortedWordCounters) {
        for (const auto& word : counter.second) {
            ostream << counter.first << " ";
            ostream << word << std::endl;
        }
    }
    return ostream;
}

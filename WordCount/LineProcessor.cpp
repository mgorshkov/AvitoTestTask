//
//  LineProcessor.cpp
//  WordCount
//
//  Created by Mikhail Gorshkov on 26.11.2021.
//  Copyright © 2021 Mikhail Gorshkov. All rights reserved.
//

#include "LineProcessor.hpp"

LineProcessor::LineProcessor(WordCollector* collector)
    : m_wordCollector{collector}{
    
}
    
void LineProcessor::processBuf(const char* buf, std::size_t size) {
    auto ScanSpaces = [buf](auto start, auto end) {
        for (auto pos = start; pos < end; ++pos) {
            if (isalpha(buf[pos]))
                return pos;
        }
        return std::string::npos;
    };
    auto ScanWord = [buf](auto start, auto end, auto& word) {
        word.clear();
        for (auto pos = start; pos < end; ++pos) {
            if (isalpha(buf[pos])) {
                word += tolower(buf[pos]);
            } else {
                return pos;
            }
        }
        return std::string::npos;
    };
    
    std::string::size_type startWordPos = 0;
    
    std::string word;
    while (true) {
        auto endWordPos = ScanWord(startWordPos, size, word);
        if (endWordPos == std::string::npos) {
            break;
        }
        if (!word.empty()) {
            m_wordCollector->collectWord(word);
            word.clear();
        }
        startWordPos = ScanSpaces(endWordPos, size);
        if (startWordPos == std::string::npos) {
            break;
        }
    }
    if (!word.empty()) {
        m_wordCollector->collectWord(word);
    }
}


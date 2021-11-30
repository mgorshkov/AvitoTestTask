//
//  LineProcessor.hpp
//  WordCount
//
//  Created by Mikhail Gorshkov on 26.11.2021.
//  Copyright © 2021 Mikhail Gorshkov. All rights reserved.
//

#ifndef LineProcessor_hpp
#define LineProcessor_hpp

#include "ILineProcessor.hpp"
#include "WordCollector.hpp"

class LineProcessor : public ILineProcessor {
public:
    LineProcessor(WordCollector* collector);
    
    void processBuf(const char* buf, std::size_t size) override;
    
private:
    WordCollector* m_wordCollector;
};

#endif /* LineProcessor_hpp */

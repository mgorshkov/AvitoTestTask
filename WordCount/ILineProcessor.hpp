//
//  ILineProcessor.hpp
//  WordCount
//
//  Created by Mikhail Gorshkov on 26.11.2021.
//  Copyright © 2021 Mikhail Gorshkov. All rights reserved.
//

#ifndef ILineProcessor_hpp
#define ILineProcessor_hpp

#include <cstddef>

class ILineProcessor {
public:
    virtual ~ILineProcessor() = default;
    
    virtual void processBuf(const char* buf, std::size_t size) = 0;
};

#endif /* ILineProcessor_hpp */

#ifndef BITMAPBASE_H
#define BITMAPBASE_H

#include <vector>
#include <cstdint>

struct BitmapBase {
    virtual ~BitmapBase() = default;
    
    int width;
    int height;
    std::vector<uint8_t> data;
};

#endif // BITMAPBASE_H

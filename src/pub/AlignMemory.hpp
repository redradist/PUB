/**
 * @file AlignMemory.hpp
 * @author Denis Kotov
 * @date 7 Okt 2018
 * @brief Contains description of AlignMemory
 * @copyright MIT License. Open source: https://github.com/redradist/PUB.git
 */

#ifndef BUFFERS_ALIGNMEMORY_HPP
#define BUFFERS_ALIGNMEMORY_HPP

namespace buffers {

enum class AlignMemory {
  Bits_8 = 1,
  Bits_16 = 2,
  Bits_32 = 4,
  Bits_64 = 8,
};

}

#endif //BUFFERS_ALIGNMEMORY_HPP

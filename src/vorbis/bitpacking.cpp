#include "vorbis/bitpacking.h"
#include <vector>

namespace Vorbis
{
    Bitstream::Bitstream()
    {
        mStream = std::vector<byte>(); 
    }

    Bitstream::~Bitstream()
    {

    }
}

#include "vorbis/helpers.hpp"

namespace Vorbis
{
    int ilog(int x)
    {
        int ret = 0;
        while (x > 0) {
            ret++;
            x >>= 1; 
        }

        return ret;
    }
}

#include "oggexceptions.h"

namespace OggException
{
    const char* InvalidCapturePattern::what() const throw()
    {
        return "Invalid Capture Pattern"; 
    }

    const char* InterferingApplicationType::what() const throw()
    {
        return "Interfering Application Type";   
    }
    
    const char* UnsupportedCodecType::what() const throw()
    {
        return "Unsupported Codec Type";   
    }
    
    const char* CodecTypeNotFound::what() const throw()
    {
        return "Codec Type Not Found"; 
    }
}

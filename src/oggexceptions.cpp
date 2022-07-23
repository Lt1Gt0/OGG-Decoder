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
    
    const char* UnknownApplicationType::what() const throw()
    {
        return "Interfering Application Type";   
    }
}

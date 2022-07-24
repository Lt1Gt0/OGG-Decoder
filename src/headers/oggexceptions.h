#pragma once
#ifndef _OGG_EXPECTIONS_H
#define _OGG_EXPECTIONS_H

#include <exception>

namespace OggException
{
    static class InvalidCapturePattern : public std::exception
    {
        virtual const char* what() const throw();
    } invalid_capture_pattern;
    
    static class InterferingApplicationType : public std::exception
    {
        virtual const char* what() const throw();
    } interfering_application_type;
    
    static class UnsupportedCodecType : public std::exception
    {
        virtual const char* what() const throw();
    } unsupported_codec_type;
    
    static class CodecTypeNotFound : public std::exception
    {
        virtual const char* what() const throw();
    } codec_type_not_found;
}

#endif // _OGG_EXCEPTIONS_H

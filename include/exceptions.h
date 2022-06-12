#pragma once
#include <exception>

class NoFieldException: public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "file is empty ...";
    }
};

class FileOpenException: public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "file can not be open ...";
    }
};
class FileMissingException: public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "file is missing ...";
    }
};


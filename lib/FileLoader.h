#ifndef FILELOADER_H
#define FILELOADER_H

#include <exception>
#include <string>

#include "GL/glew.h"

namespace CS123 {

class IOException : public std::exception {
public:
    IOException(const std::string &what) : message(what) {}
    virtual ~IOException() throw() {}
    virtual const char* what() const throw() override { return message.c_str(); }

private:
    std::string message;
};

}

class FileLoader
{
public:
    static std::string loadResourceFileToString(const std::string &resourcePath);
};

#endif // FILELOADER_H

#ifndef FILE_READER_UTIL_H
#define FILE_READER_UTIL_H
#include <string>
#include <vector>

class FileReaderUtil
{
    public:
        FileReaderUtil();
        virtual ~FileReaderUtil();
        static std::string ReadFileAsString(const std::string& filepath);
        static std::vector<int> GetMapValues(const std::string& str);
};
#endif
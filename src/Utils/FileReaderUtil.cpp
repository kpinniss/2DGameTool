#include "FileReaderUtil.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

std::string FileReaderUtil::ReadFileAsString(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        // Handle file open error
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    file.close();
    return content;
}

std::vector<int> FileReaderUtil::GetMapValues(const std::string& str) {
    std::vector<int> result;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, ',')) {
        try {
            result.push_back(std::stoi(item));
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid number: " << item << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Number out of range: " << item << std::endl;
        }
    }

    return result;
}
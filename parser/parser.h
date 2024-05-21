#ifndef IMAGE_PROCESSOR_PARSER_H
#define IMAGE_PROCESSOR_PARSER_H

#include <unordered_map>
#include <string>
#include "../help_objects.h"

class ParserInputData {
private:
    std::unordered_map<std::string, int> filter_restrictions_;
    const int min_num_ = 3;

public:
    ParserInputData();

    ParserData Parse(int argc, char *argv[]);
};

#endif

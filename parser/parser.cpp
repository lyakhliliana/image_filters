#include <cstddef>
#include <stdexcept>
#include "parser.h"

ParserInputData::ParserInputData() {
    filter_restrictions_ = {{"-crop", 2}, {"-gs", 0},   {"-neg", 0}, {"-sharp", 0},
                            {"-edge", 1}, {"-blur", 1}, {"-new", 0}};
}

ParserData ParserInputData::Parse(int argc, char *argv[]) {
    if (argc < min_num_) {
        throw std::invalid_argument("More than two items expected.");
    }

    ParserData data;
    data.file_input_ = argv[1];
    data.file_output_ = argv[2];
    int counter = min_num_;
    while (counter < argc) {
        if (argv[counter][0] == '-') {
            if (filter_restrictions_.find(argv[counter]) == filter_restrictions_.end()) {
                throw std::invalid_argument("Invalid filter name.");
            }
            std::vector<std::string_view> params = {};
            if (counter + filter_restrictions_[argv[counter]] > argc) {
                throw std::invalid_argument("Not enough filter options.");
            }
            for (size_t i = 0; i < filter_restrictions_[argv[counter]]; ++i) {
                params.emplace_back(argv[counter + i + 1]);
            }
            FilterDescriptor new_fd{.name = argv[counter], .par = params};
            counter += filter_restrictions_[argv[counter]] + 1;
            data.filters_.emplace_back(new_fd);
        } else {
            throw std::invalid_argument("Too many filter options.");
        }
    }
    return data;
}
#ifndef IMAGE_PROCESSOR_FILTER_PIPELINE_H
#define IMAGE_PROCESSOR_FILTER_PIPELINE_H

#include <vector>
#include <memory>
#include "../help_objects.h"
#include "../Image/image.h"
#include "../Filters/filters.h"

class FilterPipeline {
private:
    std::vector<std::unique_ptr<BaseFilter>> filters_;

public:
    explicit FilterPipeline(const ParserData &args);

    void ApplyFilters(Image &image);
};

#endif
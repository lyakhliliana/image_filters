#include "filter_pipeline.h"
#include <string>

FilterPipeline::FilterPipeline(const ParserData &args) {
    for (const auto &[name, params] : args.filters_) {
        if (name == "-crop") {
            filters_.emplace_back(
                std::make_unique<CropFilter>(CropFilter(std::stoi(static_cast<const std::string>(params[0])),
                                                        std::stoi(static_cast<const std::string>(params[1])))));
        } else if (name == "-gs") {
            filters_.emplace_back(std::make_unique<GrayScaleFilter>(GrayScaleFilter()));
        } else if (name == "-neg") {
            filters_.emplace_back(std::make_unique<NegativeFilter>(NegativeFilter()));
        } else if (name == "-sharp") {
            filters_.emplace_back(std::make_unique<SharpeningFilter>(SharpeningFilter()));
        } else if (name == "-edge") {
            filters_.emplace_back(std::make_unique<EdgeDetectionFilter>(
                EdgeDetectionFilter(std::stod(static_cast<std::string>(params[0])))));
        } else if (name == "-blur") {
            filters_.emplace_back(std::make_unique<GaussianBlurFilter>(
                GaussianBlurFilter(std::stod(static_cast<std::string>(params[0])))));
        } else if (name == "-new") {
            filters_.emplace_back(std::make_unique<NewFilter>(NewFilter()));
        }
    }
}

void FilterPipeline::ApplyFilters(Image &image) {
    for (const auto &filter : filters_) {
        filter->Apply(image);
    }
}

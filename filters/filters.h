#ifndef IMAGE_PROCESSOR_FILTERS_H
#define IMAGE_PROCESSOR_FILTERS_H

#include "../Parser/parser.h"
#include "../Image/image.h"
#include <stdexcept>
#include <vector>
#include <unordered_map>

using Matrix = std::vector<std::vector<double>>;

class BaseFilter {
public:
    virtual void Apply(Image &image) = 0;

    virtual ~BaseFilter() = default;
};

class ConvolutionFilter : public BaseFilter {
public:
    Matrix matrix;

    void MatrixApply(Image &image);
};

class CropFilter : public BaseFilter {
public:
    CropFilter(int32_t w, int32_t h);

    void Apply(Image &image) override;

private:
    int width_;
    int height_;
};

class GrayScaleFilter : public BaseFilter {
public:
    void Apply(Image &image) override;
};

class NegativeFilter : public BaseFilter {
public:
    void Apply(Image &image) override;
};

class SharpeningFilter : public ConvolutionFilter {
public:
    SharpeningFilter();

    void Apply(Image &image) override;
};

class EdgeDetectionFilter : public ConvolutionFilter {
public:
    explicit EdgeDetectionFilter(double value);

    void Apply(Image &image) override;

private:
    double threshold_{};
};

class GaussianBlurFilter : public ConvolutionFilter {
public:
    explicit GaussianBlurFilter(double value);

    void Apply(Image &image) override;

private:
    double sigma_;
};

class NewFilter : public ConvolutionFilter {
public:
    NewFilter();

    void Apply(Image &image) override;
};

#endif
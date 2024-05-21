#include <math.h>
#include <cstdint>
#include <functional>
#include <cmath>
#include "filters.h"

CropFilter::CropFilter(int32_t w, int32_t h) : width_(w), height_(h) {
}

SharpeningFilter::SharpeningFilter() {
    const double five = 5.;
    matrix = {{0., -1., 0.}, {-1., five, -1.}, {0., -1., 0.}};
}

EdgeDetectionFilter::EdgeDetectionFilter(double value) {
    threshold_ = value;
    const double four = 4.;
    matrix = {{0., -1., 0.}, {-1., four, -1.}, {0., -1., 0.}};
}

GaussianBlurFilter::GaussianBlurFilter(double value) {
    sigma_ = value;
    int radius = std::ceil(sigma_ * 3);
    int size = radius + radius + 1;
    matrix.resize(size, std::vector<double>(size));
    double sum = 0;
    const double two = 2.;
    for (int x = -radius; x <= radius; ++x) {
        for (int y = -radius; y <= radius; ++y) {
            double exponential = std::exp(-1.0 * (x * x + y * y) / (two * sigma_ * sigma_));
            double square = 2 * M_PI * sigma_ * sigma_;
            double weight = exponential / square;
            matrix[x + radius][y + radius] = weight;
            sum += matrix[x + radius][y + radius];
        }
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] /= sum;
        }
    }
}

void ConvolutionFilter::MatrixApply(Image &image) {
    int m_rad = static_cast<int>(matrix.size() / 2);
    int width = static_cast<int>(image.GetColorData().GetWidth());
    int height = static_cast<int>(image.GetColorData().GetHeight());
    ColorData new_data = image.GetColorData();
    const int two_five_five = 255;
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            double color[3]{0, 0, 0};
            for (int i = 0; i <= 2 * m_rad; ++i) {
                for (int j = 0; j <= 2 * m_rad; ++j) {
                    int temp_x = std::clamp(x - m_rad + i, 0, width - 1);
                    int temp_y = std::clamp(y - m_rad + j, 0, height - 1);
                    color[0] += matrix[i][j] * image.GetColorData()(temp_x, temp_y).red;
                    color[1] += matrix[i][j] * image.GetColorData()(temp_x, temp_y).green;
                    color[2] += matrix[i][j] * image.GetColorData()(temp_x, temp_y).blue;
                }
            }
            new_data(x, y).red = std::min(std::max(static_cast<int>(color[0]), 0), two_five_five);
            new_data(x, y).green = std::min(std::max(static_cast<int>(color[1]), 0), two_five_five);
            new_data(x, y).blue = std::min(std::max(static_cast<int>(color[2]), 0), two_five_five);
        }
    }
    image.GetColorData() = new_data;
}

void CropFilter::Apply(Image &image) {
    int height = static_cast<int>(image.GetColorData().GetHeight());
    int width = static_cast<int>(image.GetColorData().GetWidth());
    width_ = std::min(width_, width);
    height_ = std::min(height_, height);
    ColorData new_data(width_, height_);
    for (int i = 0; i < width_; ++i) {
        for (int j = height - height_; j < height; ++j) {
            new_data(i, j - height + height_) = image.GetColorData()(i, j);
        }
    }
    image.GetColorData() = new_data;
    image.SetNewSizeBMP(width_, height_);
}

void GrayScaleFilter::Apply(Image &image) {
    int height = static_cast<int>(image.GetColorData().GetHeight());
    int width = static_cast<int>(image.GetColorData().GetWidth());
    for (int i = 0; i < height * width; ++i) {
        const double const_for_red = 0.299;
        const double const_for_green = 0.587;
        const double const_for_blue = 0.114;
        Color color = image.GetColorData()[i];
        double new_value = const_for_red * color.red + const_for_green * color.green + const_for_blue * color.blue;
        auto new_value_uint = static_cast<uint8_t>(new_value);
        image.GetColorData()[i].red = new_value_uint;
        image.GetColorData()[i].green = new_value_uint;
        image.GetColorData()[i].blue = new_value_uint;
    }
}

void NegativeFilter::Apply(Image &image) {
    int height = static_cast<int>(image.GetColorData().GetHeight());
    int width = static_cast<int>(image.GetColorData().GetWidth());
    const int two_five_five = 255;
    for (size_t i = 0; i < width * height; ++i) {
        Color color = image.GetColorData()[i];
        color.red = two_five_five - color.red;
        color.green = two_five_five - color.green;
        color.blue = two_five_five - color.blue;
        image.GetColorData()[i] = color;
    }
}

void SharpeningFilter::Apply(Image &image) {
    MatrixApply(image);
}

void EdgeDetectionFilter::Apply(Image &image) {
    GrayScaleFilter().Apply(image);
    MatrixApply(image);

    int height = static_cast<int>(image.GetColorData().GetHeight());
    int width = static_cast<int>(image.GetColorData().GetWidth());

    const int two_five_five = 255;

    for (size_t i = 0; i < height * width; ++i) {

        if (image.GetColorData()[i].red > threshold_) {
            image.GetColorData()[i].red = two_five_five;
            image.GetColorData()[i].green = two_five_five;
            image.GetColorData()[i].blue = two_five_five;
        } else {
            image.GetColorData()[i].red = 0;
            image.GetColorData()[i].green = 0;
            image.GetColorData()[i].blue = 0;
        }
    }
}

void GaussianBlurFilter::Apply(Image &image) {
    MatrixApply(image);
}

void NewFilter::Apply(Image &image) {
    MatrixApply(image);
}

NewFilter::NewFilter() {
    const double thirteen = 13;
    matrix = {{0., 0., 1. / thirteen, 0., 0.},
              {0., 1. / thirteen, 1. / thirteen, 1. / thirteen, 0},
              {1. / thirteen, 1. / thirteen, 1., 1. / thirteen, 1. / thirteen},
              {0., 1. / thirteen, 1. / thirteen, 1. / thirteen, 0},
              {0., 0., 1. / thirteen, 0., 0.}};
}
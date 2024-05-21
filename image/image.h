#ifndef IMAGE_PROCESSOR_IMAGE_H
#define IMAGE_PROCESSOR_IMAGE_H

#include "../help_objects.h"

class ColorData {
public:
    ColorData() : storage_(nullptr), width_(0), height_(0) {
    }

    ColorData(size_t width, size_t height);

    ColorData &operator=(const ColorData &other);

    ColorData(const ColorData &other);

    ~ColorData();

    Color &operator[](size_t ind);

    const Color &operator[](size_t ind) const;

    Color &operator()(size_t x, size_t y);

    const Color &operator()(size_t x, size_t y) const;

    size_t GetWidth() const;

    size_t GetHeight() const;

private:
    Color *storage_;
    size_t width_;
    size_t height_;
};

class Image {
private:
    BMPHeader bmp_header_;
    BMPInfo bmp_info_;
    ColorData color_data_;

public:
    bool Load(std::string_view file_name);

    bool Save(std::string_view file_name);

    void SetNewSizeBMP(int32_t width, int32_t height);

    ColorData &GetColorData();
};

#endif

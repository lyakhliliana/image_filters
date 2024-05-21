#include "image.h"
#include <fstream>
#include <iostream>
#include <string_view>

ColorData::ColorData(size_t width, size_t height) {
    width_ = width;
    height_ = height;
    if (!height_ || !width_) {
        width_ = height_ = 0;
        storage_ = nullptr;
        return;
    }
    storage_ = new Color[width_ * height_];
    for (size_t i = 0; i < width_ * height_; ++i) {
        storage_[i] = {0, 0, 0};
    }
}

ColorData::ColorData(const ColorData &other) {
    width_ = other.width_;
    height_ = other.height_;
    if (!other.storage_) {
        storage_ = nullptr;
        width_ = height_ = 0;
        return;
    }
    storage_ = new Color[width_ * height_];
    for (size_t i = 0; i < height_ * width_; ++i) {
        storage_[i] = other.storage_[i];
    }
}

ColorData &ColorData::operator=(const ColorData &other) {
    if (this == &other) {
        return *this;
    }
    ColorData new_data(other);
    std::swap(width_, new_data.width_);
    std::swap(storage_, new_data.storage_);
    std::swap(height_, new_data.height_);
    return *this;
}

ColorData::~ColorData() {
    delete[] storage_;
}

Color &ColorData::operator()(size_t x, size_t y) {
    if (y > height_ || x > width_) {
        throw std::out_of_range("Invalid args");
    }
    return storage_[y * width_ + x];
}

const Color &ColorData::operator()(size_t x, size_t y) const {
    if (y > height_ || x > width_) {
        throw std::out_of_range("Invalid args");
    }
    return storage_[y * width_ + x];
}

size_t ColorData::GetWidth() const {
    return width_;
}

size_t ColorData::GetHeight() const {
    return height_;
}

Color &ColorData::operator[](size_t ind) {
    if (ind > height_ * width_) {
        throw std::out_of_range("Invalid args");
    }
    return storage_[ind];
}

const Color &ColorData::operator[](size_t ind) const {
    if (ind > height_ * width_) {
        throw std::out_of_range("Invalid args");
    }
    return storage_[ind];
}

ColorData &Image::GetColorData() {
    return color_data_;
}

void Image::SetNewSizeBMP(int32_t width, int32_t height) {
    bmp_header_.bmp_size = width * height * 3;
    bmp_info_.image_size = width * height * 3;
    bmp_info_.height = height;
    bmp_info_.width = width;
}

bool Image::Load(std::string_view file_name) {
    std::string str(file_name);
    std::fstream file;
    file.open(str, std::ios_base::in | std::ios_base::binary);
    if (!file.is_open()) {
        return false;
    }

    file.read(reinterpret_cast<char *>(&bmp_header_), sizeof(bmp_header_));
    const uint16_t for_check_bmp = 0x4D42;
    if (bmp_header_.format_signature != for_check_bmp) {
        return false;
    }
    file.read(reinterpret_cast<char *>(&bmp_info_), sizeof(bmp_info_));

    uint32_t padding = (4 - (bmp_info_.width * 3) % 4) % 4;

    ColorData storage(bmp_info_.width, bmp_info_.height);
    Color color = {0, 0, 0};
    for (size_t i = 0; i < storage.GetHeight(); ++i) {
        for (size_t j = 0; j < storage.GetWidth(); ++j) {
            file.read(reinterpret_cast<char *>(&color), sizeof(color));
            storage(j, i) = color;
        }
        file.ignore(padding);
    }
    color_data_ = storage;
    return true;
}

bool Image::Save(std::string_view file_name) {
    size_t width = color_data_.GetWidth();
    size_t height = color_data_.GetHeight();
    std::ofstream f;
    std::string str(file_name);
    f.open(str, std::ios_base::out | std::ios_base::binary);
    if (!f.is_open()) {
        return false;
    }

    f.write(reinterpret_cast<char *>(&bmp_header_), sizeof(bmp_header_));
    f.write(reinterpret_cast<char *>(&bmp_info_), sizeof(bmp_info_));
    uint32_t padding = (4 - (bmp_info_.width * 3) % 4) % 4;
    unsigned char black_paddings[3] = {0, 0, 0};
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            Color color = color_data_(j, i);
            f.write(reinterpret_cast<char *>(&color), sizeof(color));
        }
        f.write(reinterpret_cast<char *>(&black_paddings), padding);
    }
    return true;
}

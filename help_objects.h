#pragma once

#include <string_view>
#include <vector>

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    Color() : red(0), green(0), blue(0) {
    }

    Color(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b) {
    }
};

struct FilterDescriptor {
    std::string_view name;
    std::vector<std::string_view> par;
};

struct ParserData {
    std::string_view file_input_;
    std::string_view file_output_;
    std::vector<FilterDescriptor> filters_;
};

struct BMPHeader {
    uint16_t format_signature;
    uint32_t bmp_size;
    uint16_t reserved_1;
    uint16_t reserved_2;
    uint32_t offset;
} __attribute__((packed));

struct BMPInfo {
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t color_planes;
    uint16_t count_bit;
    uint32_t method;
    uint32_t image_size;
    int32_t hor_count;
    int32_t vert_count;
    uint32_t color_size;
    uint32_t imp_color_size;
} __attribute__((packed));

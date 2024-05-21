cmake_minimum_required(VERSION 3.21)
project(image_processor)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    image_processor
    image_processor.cpp
    help_objects.h
    Parser/parser.cpp
    Parser/parser.h
    Image/image.cpp
    Image/image.h
    FilterPipeline/filter_pipeline.cpp
    FilterPipeline/filter_pipeline.h
    Filters/filters.cpp
    Filters/filters.h
)

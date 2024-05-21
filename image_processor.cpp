#include <stdexcept>
#include "help_objects.h"
#include "Parser/parser.h"
#include "Image/image.h"
#include "FilterPipeline/filter_pipeline.h"

int main(int argc, char **argv) {
    ParserInputData parser;
    auto args = parser.Parse(argc, argv);
    Image img;
    if (!img.Load(args.file_input_)) {
        throw std::runtime_error("File opening failed.");
    }
    FilterPipeline pipeline(args);
    pipeline.ApplyFilters(img);
    if (!img.Save(args.file_output_)) {
        throw std::runtime_error("File saving failed.");
    }
}

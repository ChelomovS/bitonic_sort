#include "config.hpp"

#include <charconv>

void Config::dump(std::ostream &dump_stream) {
    dump_stream << "data size: "   << data_size_   << std::endl;
    dump_stream << "global size: " << global_size_ << std::endl;
    dump_stream << "local size: "  << local_size_  << std::endl;
    dump_stream << "kernel path: " << path_        << std::endl;
}
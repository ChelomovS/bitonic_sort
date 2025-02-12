#include "config.hpp"

Config Config::read_config(const int argc, const char** argv) {
    Config configurations;
    for (int i = 1; i < argc; ++i) {
        std::string_view argv_i = argv[i];

        auto argv_i_end = argv_i.data() + argv_i.size();
        if (argv_i.starts_with("-dsz=")) {
            auto result = std::from_chars(argv_i.data() + 5, argv_i_end, configurations.data_size_);
            if (result.ec == std::errc::invalid_argument)
                std::cerr << "Can not parse -dsz option, using default\n";
        } else if (argv_i.starts_with("-gsz=")) {
            auto result = std::from_chars(argv_i.data() + 5, argv_i_end, configurations.global_size_);
            if (result.ec == std::errc::invalid_argument)
                std::cerr << "Can not parse -gsz option, using default\n";
        } else if (argv_i.starts_with("-lsz=")) {
            auto result = std::from_chars(argv_i.data() + 5, argv_i_end, configurations.local_size_);
            if (result.ec == std::errc::invalid_argument)
                std::cerr << "Can not parse -lsz option, using default\n";
        } else if (argv_i.starts_with("-kernel=")) {
            configurations.path_ = argv[i] + 8;
        } else {
            throw option_error(argv[i]);
        }
    }
    return configurations;
}

void Config::dump(std::ostream &dump_stream) {
    dump_stream << "data size: "   << data_size_   << std::endl;
    dump_stream << "global size: " << global_size_ << std::endl;
    dump_stream << "local size: "  << local_size_  << std::endl;
    dump_stream << "kernel path: " << path_        << std::endl;
}

std::ostream& operator<<(std::ostream& dump_stream, Config configurations) {
    configurations.dump(dump_stream);
    return dump_stream;
}
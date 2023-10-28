#include "utils.hpp"
#include <filesystem>
#include <exception>

const char*executable;

void Utils::initialize(const char* argv0) {
    executable = argv0;
}

const std::string Utils::fetch(const std::string &filename) {
    if (executable == nullptr) {
        throw std::runtime_error("Call Utils::Initialize(argv0) first!");
    }
    std::filesystem::path path(executable);
    path = std::filesystem::absolute(path.parent_path());
    if (std::filesystem::exists(path/"res")) {
        return (path/"res"/filename).string();
    } else if (std::filesystem::exists(path.parent_path().parent_path()/"res")) {
        return (path.parent_path().parent_path()/"res"/filename).string();
    }
    throw std::runtime_error("Can't locate 'res' directory! Put 'res' inside " + path.string() + "(preferred) or " + path.parent_path().string());
    return std::string();
}

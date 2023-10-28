#pragma once
#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>


namespace Utils {
    /* Fetch a file from `res/` directory. */
    void initialize(const char *argv0);
    /* BUGFIX: Remember to store returned string and then call c_str()! */
    const std::string fetch(const std::string &filename);
};

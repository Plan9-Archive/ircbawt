#ifndef UTIL_HPP
#define UTIL_HPP

#include <iterator>
#include <ostream>
#include <vector>
#include <iostream>
#include <map>
#include <utility>

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, " "));
    return os;
}

#endif

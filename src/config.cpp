#include <stdexcept>
#include <iostream>

#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

#include "config.hpp"

namespace pod = boost::program_options::detail;

bool config::open(const std::string &file) {

  _file.open(file.c_str());

  if(!_file) {
//   return false;
   throw std::runtime_error("Can't read config file");
  }

  _options.insert("*");

  try {
    pod::config_file_iterator i(_file, _options), e;
    for (; i != e; ++i) {
#ifdef DEBUG
      std::cout << i->string_key << " = " << i->value[0] << std::endl;
#endif
      _parameters[i->string_key] = i->value[0];
    }

  }
  catch(std::exception& e) {
    std::cerr<<"Exception: "<<e.what()<<std::endl;
    return false;
  }

  return true;
}

bool config::open(const char *file) {
  std::string f(file);
  return open(f);
}

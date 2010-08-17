#include "IRCBot.hpp"

#include <iostream>
#include <fstream>

#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

bool IRCBot::ParseConfig(std::string &ConfigFile) {
  std::fstream f(ConfigFile.c_str());
  if(!f) {
    std::cerr << "Can't read config file " << ConfigFile << std::endl;
    return false;
  }
  
  m_Options.insert("*");
  
  boost::program_options::detail::config_file_iterator i(f, m_Options), e;
  for(; i != e; ++i) {
    m_Parameters[i->string_key] = i->value[0];
  }

  return true;
}

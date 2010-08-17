#include "IRCBot.hpp"
#include "Util.hpp"

#include <iostream>
#include <fstream>

#include <boost/program_options.hpp>
#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

namespace po = boost::program_options;

bool IRCBot::ParseConfig(std::string &ConfigFile) {
  std::string server, port, pass;
  
  po::options_description allowed_config("configuration");
  allowed_config.add_options()
    ("server", po::value<std::string>(&server), "irc server")
    ("port", po::value<std::string>(&port)->default_value("6667"), "irc server port")
    ("pass", po::value<std::string>(&pass)->default_value(""), "server password")
    ("channels", po::value< std::vector<std::string> >(), "initial irc channels")
    ("nick", po::value<std::string>(), "bot nickname")
    ("user", po::value<std::string>(), "bot username")
    ("real", po::value<std::string>(), "bot realname");

  po::variables_map config;
    
  std::fstream f(ConfigFile.c_str());
  if(!f) {
    std::cerr << "Can't read config file " << ConfigFile << std::endl;
    return false;
  } else {
    store(parse_config_file(f, allowed_config), config);
    notify(config);
  }

  m_Server = Server(server, port, pass);

/* add rest of bot setup >.< */

  return true;
}

bool IRCBot::Run(void) {
  /* run irc bot loop.. */
  return true;
}

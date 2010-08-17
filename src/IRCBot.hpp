#ifndef IRCBOT_HH
#define IRCBOT_HH

#include "Server.hpp"

#include <vector>
#include <string>
#include <map>
#include <set>

class IRCBot {
  public:
    IRCBot() {}
    bool ParseConfig(std::string &ConfigFile);
    bool Run(void);
  private:
  
    Server m_Server;
    
    std::set<std::string> m_Options;
    std::map<std::string, std::string> m_Parameters;
};

#endif

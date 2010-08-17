#ifndef IRCBOT_HH
#define IRCBOT_HH

#include <vector>
#include <string>
#include <map>
#include <set>

class IRCBot {
  public:
    IRCBot() {}
    bool ParseConfig(std::string &ConfigFile);
  private:
  
    std::string m_Server, m_Port, m_Pass;
    
    std::set<std::string> m_Options;
    std::map<std::string, std::string> m_Parameters;
};

#endif

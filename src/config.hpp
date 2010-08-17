#ifndef CONFIG_HH
#define CONFIG_HH

#include <fstream>
#include <string>
#include <map>
#include <set>

class config
{
  public:
    config() { };

    bool open(const std::string &file);
    bool open(const char *file);

    std::string &operator[](const std::string &key) { return _parameters[key]; }

  private:

    std::fstream _file;
    std::set<std::string> _options;
    std::map<std::string, std::string> _parameters;

};

#endif


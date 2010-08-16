#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>
#include <iterator>

#include <string>
#include <deque>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>

//#include <boost/regex.hpp>
#include "irc_client.hpp"
#include "config.hpp"

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(std::cout, " "));
    return os;
}


namespace po = boost::program_options;

int main(int argc, char *argv[])
{

  try
  {
    std::string config_file;
    po::options_description optslist("options");
    optslist.add_options()
      ("version,v", "print version string")
      ("help,h", "display help message")
      ("config,c", po::value<std::string>(&config_file)->default_value("irc_bot.cf"), "configuration file");


    po::variables_map opts;
    store(po::command_line_parser(argc, argv).options(optslist).run(), opts);

    if(opts.count("help")) {
      std::cout << visible << std::endl;
      return 0;
    }

    if(opts.count("version")) {
      stdd::cout << "Insert version here" << std::endl;
      return 0;
    }
    
    po::options_description config("configuration");    
    config.add_options()
      ("server", po::value<std::string>(), "irc server")
      ("port", po::value<std::string>()->default_value("6667"), "irc server port")
      ("channels", po::value< std::vector<std::string> >(), "initial irc channels")
      ("nick", po::value<std::string>(), "bot nickname")
      ("user", po::value<std::string>(), "bot username")
      ("real", po::value<std::string>(), "bot realname");
      
    po::variables_map bot_config;

    std::ifstream ifs(config_file.c_str());
    if(!ifs) {
      std::cout << "cannot open config file: " << config_file << std::endl;
      return 1;
    } else {
      store(parse_config_file(ifs, config), bot_config);
      notify(bot_config);
    }

    //~ boost::asio::io_service io_service;
    //~ boost::asio::ip::tcp::resolver resolver(io_service);
    //~ boost::asio::ip::tcp::resolver::query query(cf["server"], cf["port"]);
    //~ boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query);
    //~ irc_client c(io_service, it, cf["nick"]);
    //~ io_service.run();
  }
  catch (std::exception &e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }

  return 0;
}


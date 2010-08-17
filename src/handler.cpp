#include <iostream>
#include <boost/foreach.hpp>

#include <cctype>

#include "handler.hpp"
#include "IRCClient.hpp"

#ifdef DEBUG

void p(std::string s) {
  using namespace std;

  for(string::iterator it = s.begin(); it < s.end(); ++it)
    if(isprint(*it))
      cerr << *it;
    else
      cerr << " [" << hex << showbase << (int) *it << "] ";

//  cerr << endl;
}

#endif

void debug_msg(const ircdata &data) {
#ifdef DEBUG
  std::cerr << "-- P("; p(data.prefix); std::cerr << ')' << std::endl;
  std::cerr << "-- C("; p(data.command); std::cerr << ')' << std::endl;
//  std::cerr << "-- A:" << data.params.size() << std::endl;
  std::cerr << "-- A( ";
  BOOST_FOREACH( std::string s, data.params ) {
    std::cerr << "(";
    p(s);
    std::cerr << ") ";
  }
  std::cerr << ')' << std::endl;

#endif
}

void handler(irc_client &client, const ircdata &data) {

  debug_msg(data);

  if(data.command == "PING") {
    if(data.prefix.size() == 0) {
      //~ std::string p = "PONG " + data.trailing;
      std::string p = "PONG :" + data.params.back();
      client.write(p);
    }
  } else if (data.command == "PRIVMSG") {
//    std::cerr << "Wats dis? "; p(data.params.back());
    if (data.params.back().rfind("\x01VERSION\x01") != std::string::npos)
      client.ctcp_reply(irc_client::get_nick(data.prefix), "VERSION My crappy IRC lib");
  } else {
    std::cerr << "No handler for " << data.command << " here!" << std::endl;
  }
  return;
}


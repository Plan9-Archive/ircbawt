#ifndef IRCCLIENT_HPP
#define IRCCLIENT_HPP

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <deque>
#include <map>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <boost/function.hpp>

#include "IRCParser.hpp"
#include "handler.hpp"

class irc_client
{
public:
  typedef std::deque< std::string > irc_message_queue;
  typedef std::map<std::string, std::vector<irc_callback> > irc_callbacks;
  //~ typedef boost::function<int (const irc_client &, const ircdata &)> irc_callback;

  irc_client(boost::asio::io_service &service, boost::asio::ip::tcp::resolver::iterator it, const std::string& user)
  : _io_service(service), _socket(service), _user(user), _state(connecting), got_motd(false)
  {
    add_callback("PING", handler);
    add_callback("PRIVMSG", handler);
    add_callback("NOTICE", handler);
    boost::asio::ip::tcp::endpoint ep = *it;
    _socket.async_connect(ep, boost::bind(&irc_client::handle_connect, this, boost::asio::placeholders::error, ++it));
  }

  void add_callback(const std::string &cmd, irc_callback f)
  {
    callbacks[cmd].push_back(f);
  }

  void privmsg(const std::string &target, const std::string &msg);
  void notice(const std::string &target, const std::string &msg);
  void ctcp_req(const std::string &target, const std::string &msg);
  void ctcp_reply(const std::string &target, const std::string &msg);
  /* send a raw message */
  void write(std::string &msg);

  static std::string get_nick(const std::string &ident)
  {
    std::string result;
    size_t f;
    f = ident.find_first_of("@!");
    if(f != std::string::npos)
    {
      result.assign(ident, 0, f);
      return result;
    }

    return ident;
  }

private:
  enum irc_state { connecting, connected, disconnected };

  void handle_connect(const boost::system::error_code &error, boost::asio::ip::tcp::resolver::iterator it);
  void handle_read(const boost::system::error_code &error, std::size_t bytes_transferred);
  void do_write(const std::string &msg);
  void handle_write(const boost::system::error_code &error);
  void do_close()
  {
    _socket.close();
  }


  boost::asio::io_service &_io_service;
  boost::asio::ip::tcp::socket _socket;
  boost::asio::streambuf _in_msg;
  irc_message_queue _out_msgs;

  std::string _user;
  irc_state _state;
  bool got_motd;

  irc_callbacks callbacks;
};

#endif


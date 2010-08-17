 #include <iostream>
#include <boost/foreach.hpp>

#include "IRCClient.hpp"
#include "IRCParser.hpp"

void irc_client::privmsg(const std::string &target, const std::string &msg)
{
  std::string _msg = "PRIVMSG " + target + " :" + msg;
  write(_msg);
}

void irc_client::notice(const std::string &target, const std::string &msg)
{
  std::string _msg = "NOTICE " + target + " :" + msg;
  write(_msg);
}

void irc_client::ctcp_req(const std::string &target, const std::string &msg)
{
  std::string _msg = "\x01" + msg + "\0x1";
  privmsg(target, _msg);
}
void irc_client::ctcp_reply(const std::string &target, const std::string &msg)
{
  std::string _msg = "\x01" + msg + "\0x1";
  notice(target, _msg);
}

void irc_client::write(std::string &msg)
{
#ifdef DEBUG
  std::cout << "<< " << msg << std::endl; std::cout.flush();
#endif
  msg += "\r\n";
  _io_service.post(boost::bind(&irc_client::do_write, this, msg));
}

void irc_client::handle_connect(const boost::system::error_code &error, boost::asio::ip::tcp::resolver::iterator it)
{
  if(!error)
  {
    boost::asio::async_read_until(_socket, _in_msg, std::string("\r\n"),
      boost::bind(&irc_client::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  }
  else if (it != boost::asio::ip::tcp::resolver::iterator())
  {
    _socket.close();
    boost::asio::ip::tcp::endpoint ep = *it;
    _socket.async_connect(ep, boost::bind(&irc_client::handle_connect, this, boost::asio::placeholders::error, ++it));
  }
  else
  {
    std::cerr << "Error: " << error.message() << std::endl;
  }
}

void irc_client::handle_read(const boost::system::error_code &error, std::size_t bytes_transferred)
{

  if(!error)
  {
    std::string nick = "NICK " + _user;
    std::string user = "USER " + _user + " " + _user + " " + _user + " :" + _user;

    // Print message info

    std::istream msgstream(&_in_msg);
    std::string msg;
    std::getline(msgstream, msg);

    // do something with the message depending on state

    switch(_state)
    {
    case connecting:
      write(nick);
      write(user);
//      std::cout << "Sent nick and user.. connected! \n";
      _state = connected;
      break;
    case connected:
      {
        irc_parser<std::string::const_iterator> g;
        ircdata out;

        std::string::const_iterator begin = msg.begin();
        std::string::const_iterator end = msg.end();

        // parse it
        bool r = boost::spirit::qi::parse(begin, end, g, out);
        if (r && begin == end)
        {
          std::cout << ">> " << msg << std::endl;
          std::cout.flush();

          irc_callbacks::iterator it = callbacks.find(out.command);
          if(it != callbacks.end() )
          {
            //~ std::cerr << "Calling " << it->second.size() << "callbacks for command " << out.command << std::endl;
            // call each callback in order.
            irc_callback f;
            BOOST_FOREACH( f, it->second )
            {
              f(*this, out);
            }
          }
          else
          {
            //~ std::cerr << "No callbacks found for command " << out.command << std::endl;
          }
        }
        else
        {
            std::cout << "Parsing " << msg << " failed." << std::endl;
            std::flush(std::cout);
        }
      }
      break;

    default:
      break;
    }

    // queue a read for another message

    boost::asio::async_read_until(_socket, _in_msg, std::string("\r\n"),
      boost::bind(&irc_client::handle_read, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

  }
  else
  {
    _state = disconnected;
    do_close();
  }
}

void irc_client::do_write(const std::string &msg)
{
  bool write_in_progress = !_out_msgs.empty();
  _out_msgs.push_back(msg);
  if (!write_in_progress)
  {
    boost::asio::async_write(_socket, boost::asio::buffer(_out_msgs.front().data(), _out_msgs.front().size()),
      boost::bind(&irc_client::handle_write, this, boost::asio::placeholders::error));
  }
}

void irc_client::handle_write(const boost::system::error_code &error)
{
  if (!error)
  {
    //~ std::cerr << "Finished writing: " << _out_msgs.front() << std::endl; std::cout.flush();
    _out_msgs.pop_front();
    if (!_out_msgs.empty())
    {
      boost::asio::async_write(_socket, boost::asio::buffer(_out_msgs.front().data(), _out_msgs.front().size()),
        boost::bind(&irc_client::handle_write, this, boost::asio::placeholders::error));
    }
  }
  else
  {
    do_close();
  }
}

/*
        std::string outmsg;
        std::string regex("(PING :)(.+)");
        boost::regex pong(regex);
        boost::smatch what;
        if(regex_match(msg, what, pong))
        {
          outmsg = "PONG ";
          outmsg += what[2].str();
          write(outmsg);
        }
        else
        {
        }
*/


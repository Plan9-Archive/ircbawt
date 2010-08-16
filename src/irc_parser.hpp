#ifndef IRC_PARSER_HPP
#define IRC_PARSER_HPP

#include <string>
#include <vector>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace enc = spirit::standard;

struct ircdata
{
  std::string prefix;
  std::string command;
  std::vector<std::string> params;
};

BOOST_FUSION_ADAPT_STRUCT(
    ircdata,
    (std::string, prefix)
    (std::string, command)
    (std::vector<std::string>, params)
)

template <typename Iterator>
struct irc_parser : qi::grammar<Iterator, ircdata()>
{
  irc_parser() : irc_parser::base_type(message)
  {
    using qi::lit;
    using qi::raw;
    using spirit::repeat;
    using enc::char_;

    message = -(lit(':') >> prefix >> space) >> command >> params >> -crlf;
    prefix = raw[servername | (nick >> -(char_('!') >> user) >> -(char_('@') >> host))];
    command = +letter | repeat(3)[number];
    space = +lit(' ');
    params = *(space >> middle) >> repeat(0, 1)[space >> ':' >> trailing];
    middle = ~char_(": \r\n\0") >> *nonwhite;
    trailing = *(~char_("\r\n\0"));
    crlf = lit('\r') || lit('\n');
    servername = host >> &~lit('!');
    host = raw[(shortname % char_('.'))];
    shortname = *(char_("a-zA-Z0-9-"));
    nick = letter >> *(letter | number |special);
    user = +nonwhite;

    letter = char_("A-Za-z");
    number = char_("0-9");
    special = char_("-[]\\`^{}");
    nonwhite = ~char_(" \0\r\n");

  }

  qi::rule<Iterator, ircdata()> message;
  qi::rule<Iterator, std::vector<std::string>() > params;
  qi::rule<Iterator, std::string()> prefix, command, middle, trailing, servername, host, shortname, nick, user;
  qi::rule<Iterator, char()> letter, number, special, nonwhite;
  qi::rule<Iterator> space, crlf;

};

#endif


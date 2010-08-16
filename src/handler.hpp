#ifndef HANDLER_HPP
#define HANDLER_HPP

class irc_client;
struct ircdata;

typedef void (*irc_callback)(irc_client &client, const ircdata &data);

void handler(irc_client &client, const ircdata &data);

#endif


#include "Server.hpp"

Server::Server(const std::string Name, std::string Port, const std::string Pass, bool SSL) :
  m_Name(Name), m_Port(Port), m_Pass(Pass), m_SSL(SSL)
{
}

Server::~Server() {}

const std::string& Server::GetName() const { return m_Name; }
const std::string& Server::GetPort() const { return m_Port; }
const std::string& Server::GetPass() const { return m_Pass; }
bool Server::IsSSL() const { return m_SSL; }

std::string Server::GetString(bool IncludePassword) const {
  return m_Name + " " + std::string(m_SSL ? "+" : "") + m_Port + 
    std::string(IncludePassword ? (m_Pass.empty() ? "" : " " + m_Pass) : "");
}

bool Server::IsValidHostName(const std::string& Hostname) {
  if(Hostname.empty()) {
    return false;
  }
    
  if(Hostname.find(' ') != std::string::npos) {
    return false;
  }
    
  return true;
}

#ifndef SERVER_HPP

#include <string>

class Server {
public:
  Server(const std::string Name = "", std::string Port = "6667", const std::string Pass = "", bool SSL = false);
  ~Server();
  
  bool Validate() { return true; }
  
  const std::string& GetName() const;
  const std::string& GetPort() const;
  const std::string& GetPass() const;
  bool IsSSL() const;
  std::string GetString(bool IncludePassword = true) const;
  static bool IsValidHostName(const std::string& Hostname);
private:
protected:
  std::string m_Name, m_Port, m_Pass;
  bool m_SSL;
};
  
#endif

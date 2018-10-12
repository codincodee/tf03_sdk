#ifndef PAGE_BASE_H
#define PAGE_BASE_H

#include "export.h"
#include <memory>

class CommandEchoHandler;

class API PageBase
{
 public:
  PageBase();
  virtual ~PageBase();
  void SetCommandEchoHandler(std::shared_ptr<CommandEchoHandler> echo_handler);
 protected:
   std::shared_ptr<CommandEchoHandler> EchoHandler();
 private:
  std::shared_ptr<CommandEchoHandler> echo_handler_;
};

#endif // PAGE_BASE_H

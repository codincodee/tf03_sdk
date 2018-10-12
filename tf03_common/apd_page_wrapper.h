#ifndef APD_PAGE_WRAPPER_H
#define APD_PAGE_WRAPPER_H

#include "export.h"
#include "page_base.h"

class APDPage;

class API APDPageWrapper : public PageBase
{
public:
  APDPageWrapper();
  void SetAPDPageCore(std::shared_ptr<APDPage> core);
 private:
  std::shared_ptr<APDPage> core_;
};

#endif // APD_PAGE_WRAPPER_H

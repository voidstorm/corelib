#pragma once
#include "logger.h"

//scl = schnarzcorelib
//sthread

namespace scl {

class SystemLogger {
public:

   SystemLogger() {

   }

   ~SystemLogger() {

   }

   static Logger & get() {
#ifdef _DEBUG
      static auto log = std::make_unique<Logger>("schnarzmaschine.log", true, true, true);
#else
      static auto log = std::make_unique<Logger>("schnarzmaschine.log");
#endif
      return *(log.get());
   }
};
}

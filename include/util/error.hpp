#ifndef UTIL_ASSERT_HPP
#define UTIL_ASSERT_HPP

// Includes

#include <iostream>

// Error functions

#undef assert

namespace err {
   [[noreturn]] inline void raise(std::string_view msg) {
      std::cerr << "\nAn error occurred:\n";
      std::cerr << msg << '\n';
      std::exit(-1);
   }

   inline void assert(bool condition, std::string_view msg) {
      if (not condition) {
         raise(msg);
      }
   }
}

#endif

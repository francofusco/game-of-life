#pragma once

#ifdef DEBUG_ON
  #include <iostream>
  #define DEBUG(x) std::cout << "[DEBUG] " << x << std::endl;
#else
  #define DEBUG(x) ;
#endif

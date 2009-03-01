// hack for the usleep/Sleep problem
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#  include <windows.h>
#  define usleep(x) Sleep((x)/1000)
#else
#  include <unistd.h>
#endif

// hack for the usleep/Sleep problem
#if ORXONOX_PLATFORM == ORXONOX_PLATFORM_WIN32
#  include <windows.h>
#  define usleep(x) Sleep((x)/1000)
#elif ORXONOX_PLATFORM == ORXONOX_PLATFORM_LINUX
#  include <unistd.h>
#endif

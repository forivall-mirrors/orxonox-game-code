#ifndef _FDWATCHER_H__
#define _FDWATCHER_H__

#include "objects/Tickable.h"

#include <map>
#include <poll.h>

namespace orxonox
{
    class _OrxonoxExport FDWatcher : public Tickable
    {
        typedef void (*intfunction) (int fd);

        public:
            FDWatcher();
            ~FDWatcher();

            virtual void tick(const float dt);

            void add(const int fd, intfunction cb);
            void remove(const int fd);

        private:
            std::map<int, intfunction> watches;
            pollfd * pollfds;
            int npollfds;

            void rebuild();
    };
}

#endif /* _FDWATCHER_H__ */

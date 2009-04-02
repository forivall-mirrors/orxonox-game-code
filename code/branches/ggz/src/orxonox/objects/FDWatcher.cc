#include "FDWatcher.h"

namespace orxonox
{
    FDWatcher::FDWatcher()
    {
        pollfds = 0;
        npollfds = 0;
    }

    FDWatcher::~FDWatcher()
    {
        if (pollfds) {
            delete [] pollfds;
        }
    }

    void FDWatcher::tick(const float dt)
    {
        int ret = poll(pollfds, npollfds, 0);
        if (ret < 0) {
            // TODO error
        }
        for (int i=0; ret>0; i++, ret--) {
            if (pollfds[i].revents & POLLIN) {
                (*watches[pollfds[i].fd])(pollfds[i].fd);
            }
            if (pollfds[i].revents & !POLLIN) {
                // TODO error
            }
        }
    }

    void FDWatcher::add(const int fd, intfunction cb)
    {
        watches[fd] = cb;
        npollfds++;
        rebuild();
    }

    void FDWatcher::remove(const int fd)
    {
        watches.erase(fd);
        npollfds--;
        rebuild();
    }

    void FDWatcher::rebuild()
    {
        if (pollfds) {
            delete [] pollfds;
        }
        if (npollfds) {
            pollfds = new struct pollfd[npollfds];
        }
        else {
            pollfds = 0;
        }
        int i=0;
        for (std::map<int, intfunction>::iterator it=watches.begin();
                it!=watches.end(); it++, i++)
        {
            pollfds[i].fd = it->first;
            pollfds[i].events = POLLIN;
        }
    }
}

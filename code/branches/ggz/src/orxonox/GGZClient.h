#ifndef _GGZClient_H__
#define _GGZClient_H__

#include "OrxonoxPrereqs.h"
#include "objects/FDWatcher.h"

#include <ggzmod.h>

namespace orxonox
{
    class _OrxonoxExport GGZClient
    {
        public:
            GGZClient();
            ~GGZClient();

            static GGZClient& getInstance();

        private:
            static GGZClient* singletonRef_s;

            bool active;
            GGZMod *ggzmod;
            FDWatcher sockets;

            void initGGZ();
            void deinitGGZ();
            static void handleGame(int fd);
            static void handleGGZ(int fd);
            static void handleGGZModServer(GGZMod * ggzmod, GGZModEvent e,
                    const void *data);
    };
}

#endif /* _GGZClient_H__ */

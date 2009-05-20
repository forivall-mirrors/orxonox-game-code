#ifndef _GGZClient_H__
#define _GGZClient_H__

#include "OrxonoxPrereqs.h"
#include "objects/Tickable.h"

#include <ggzmod.h>
#include <boost/asio.hpp>

namespace orxonox
{
    class _OrxonoxExport GGZClient : public Tickable
    {
        public:
            GGZClient();
            ~GGZClient();

            static GGZClient& getInstance();
            virtual void tick(const float dt);

        private:
            static GGZClient* singletonRef_s;

            bool active;
            GGZMod * ggzmod;
            boost::asio::io_service io;
            boost::asio::ip::tcp::socket ggzSocket;
            boost::asio::ip::tcp::socket gameSocket;

            void initGGZ();
            void deinitGGZ();
            void handleGame(const boost::system::error_code& e);
            void handleGGZ(const boost::system::error_code& e);
            static void handleGGZModServer(GGZMod * ggzmod, GGZModEvent e,
                    const void *data);
    };
}

#endif /* _GGZClient_H__ */

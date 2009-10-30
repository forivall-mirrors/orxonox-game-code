#ifndef _GGZClient_H__
#define _GGZClient_H__

#include "OrxonoxPrereqs.h"
#include "tools/interfaces/Tickable.h"
#include "gamestates/GSClient.h"

#include <ggzmod.h>
#include <boost/asio.hpp>

namespace orxonox
{
    class _OrxonoxExport GGZClient : public Tickable
    {
        public:
            GGZClient(GSClient * client);
            ~GGZClient();

            static GGZClient& getInstance();
            static bool isActive();
            virtual void tick(const float dt);

        private:
            static GGZClient* singletonRef_s;

            GSClient * client;
            GGZMod * ggzmod;
            boost::asio::io_service io;
            boost::asio::local::stream_protocol::socket ggzSocket;
            boost::asio::local::stream_protocol::socket gameSocket;

            void initGGZ();
            void deinitGGZ();
            static void handleGame(const boost::system::error_code& e);
            static void handleGGZ(const boost::system::error_code& e);
            static void handleGGZModServer(GGZMod * ggzmod, GGZModEvent e,
                    const void *data);
    };
}

#endif /* _GGZClient_H__ */

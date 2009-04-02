#include "GGZClient.h"

#include <cassert>

namespace orxonox
{
    GGZClient* GGZClient::singletonRef_s = 0;

    GGZClient::GGZClient()
    {
        assert(singletonRef_s == 0);
        singletonRef_s = this;

        active = ggzmod_is_ggz_mode();
        if (active) {
            initGGZ();
        }
    }

    GGZClient::~GGZClient()
    {
        if (active) {
            deinitGGZ();
        }

        assert(singletonRef_s);
        singletonRef_s = 0;
    }

    GGZClient& GGZClient::getInstance()
    {
        assert(singletonRef_s);
        return *singletonRef_s;
    }

    void GGZClient::initGGZ()
    {
        ggzmod = ggzmod_new(GGZMOD_GAME);
        ggzmod_set_handler(ggzmod, GGZMOD_EVENT_SERVER,
                &orxonox::GGZClient::handleGGZModServer);
        if (ggzmod_connect(ggzmod) < 0) {
            /* TODO: Error */
        }
        int ggzSocket = ggzmod_get_fd(ggzmod);
        if (ggzSocket < 0) {
            /* TODO: Error */
        }
        sockets.add(ggzSocket, &orxonox::GGZClient::handleGGZ);
    }

    void GGZClient::deinitGGZ()
    {
        ggzmod_disconnect(ggzmod);
        ggzmod_free(ggzmod);
    }

    /* Got data from game server */
    void handleGame(int fd)
    {
        /* TODO: read from fd */
    }

    /* Got data from GGZ */
    void GGZClient::handleGGZ(int fd)
    {
        ggzmod_dispatch(getInstance().ggzmod);
    }

    /* Connection to game server established */
    void GGZClient::handleGGZModServer(GGZMod * ggzmod, GGZModEvent e,
            const void *data)
    {
        ggzmod_set_state(ggzmod, GGZMOD_STATE_PLAYING);
        int gameSocket = *(int*)data;
        getInstance().sockets.add(gameSocket, &orxonox::GGZClient::handleGGZ);
    }
}

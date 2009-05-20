#include "GGZClient.h"

#include <cassert>

namespace orxonox
{
    GGZClient* GGZClient::singletonRef_s = 0;

    GGZClient::GGZClient()
        : ggzSocket(io), gameSocket(io)
    {
        assert(singletonRef_s == 0);
        singletonRef_s = this;

        boost::asio::ip::tcp::socket::non_blocking_io non_blocking_io(true);
        ggzSocket.io_control(non_blocking_io);
        gameSocket.io_control(non_blocking_io);

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

    void GGZClient::tick(const float /*dt*/)
    {
        boost::system::error_code ec;
        io.poll(ec);
        if (ec) {
            /* TODO: Error */
        }
    }

    void GGZClient::initGGZ()
    {
        ggzmod = ggzmod_new(GGZMOD_GAME);
        ggzmod_set_handler(ggzmod, GGZMOD_EVENT_SERVER,
                &orxonox::GGZClient::handleGGZModServer);
        if (ggzmod_connect(ggzmod) < 0) {
            /* TODO: Error */
        }
        int fd = ggzmod_get_fd(ggzmod);
        if (fd < 0) {
            /* TODO: Error */
        }
        /* TODO: Error */
        ggzSocket.assign(boost::asio::ip::tcp::v4(), fd);
        ggzSocket.async_read_some(boost::asio::null_buffers(), handleGGZ);
    }

    void GGZClient::deinitGGZ()
    {
        ggzmod_disconnect(ggzmod);
        ggzmod_free(ggzmod);
    }

    /* Got data from game server */
    void handleGame(const boost::system::error_code& /*e*/)
    {
        /* TODO: read from gameSocket */
    }

    /* Got data from GGZ */
    void GGZClient::handleGGZ(const boost::system::error_code& /*e*/)
    {
        ggzmod_dispatch(getInstance().ggzmod);
    }

    /* Connection to game server established */
    void GGZClient::handleGGZModServer(GGZMod * ggzmod, GGZModEvent e,
            const void *data)
    {
        ggzmod_set_state(ggzmod, GGZMOD_STATE_PLAYING);
        gameSocket.assign(boost::asio::ip::tcp::v4(), *(int*)data);
        gameSocket.async_read_some(boost::asio::null_buffers(), handleGame);
    }
}

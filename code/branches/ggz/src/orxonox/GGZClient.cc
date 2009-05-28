#include "GGZClient.h"

#include <cassert>
#include <boost/bind.hpp>

namespace orxonox
{
    GGZClient* GGZClient::singletonRef_s = 0;

    GGZClient::GGZClient(GSClient * c)
        : ggzSocket(io), gameSocket(io)
    {
        assert(singletonRef_s == 0);
        singletonRef_s = this;

        client = c;
        initGGZ();
    }

    GGZClient::~GGZClient()
    {
        deinitGGZ();

        assert(singletonRef_s);
        singletonRef_s = 0;
    }

    GGZClient& GGZClient::getInstance()
    {
        assert(singletonRef_s);
        return *singletonRef_s;
    }

    bool GGZClient::isActive()
    {
        return ggzmod_is_ggz_mode();
    }

    void GGZClient::tick(const float /*dt*/)
    {
        COUT(3) << "Tick\n";
        boost::system::error_code ec;
        io.poll(ec);
        if (ec)
        {
            /* TODO: Error */
        }
    }

    void GGZClient::initGGZ()
    {
        COUT(3) << "Initializing GGZ\n";
        ggzmod = ggzmod_new(GGZMOD_GAME);
        ggzmod_set_handler(ggzmod, GGZMOD_EVENT_SERVER,
                &orxonox::GGZClient::handleGGZModServer);
        if (ggzmod_connect(ggzmod) < 0)
        {
            /* TODO: Error */
        }
        int fd = ggzmod_get_fd(ggzmod);
        if (fd < 0)
        {
            /* TODO: Error */
        }
        /* TODO: Error */
        ggzSocket.assign(boost::asio::local::stream_protocol(), fd);
        ggzSocket.async_read_some(boost::asio::null_buffers(), boost::bind(&handleGGZ, boost::asio::placeholders::error));
    }

    void GGZClient::deinitGGZ()
    {
        ggzmod_disconnect(ggzmod);
        ggzmod_free(ggzmod);
    }

    /* Got data from game server */
    void GGZClient::handleGame(const boost::system::error_code& /*e*/)
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
        COUT(3) << "GGZ Initialized\n";
        ggzmod_set_state(ggzmod, GGZMOD_STATE_PLAYING);
        getInstance().gameSocket.assign(boost::asio::local::stream_protocol(), *(int*)data);
        getInstance().gameSocket.async_read_some(boost::asio::null_buffers(), boost::bind(&handleGame, boost::asio::placeholders::error));
    }
}

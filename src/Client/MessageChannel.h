//
// Created by Walker Crouse on 9/26/18.
//

#ifndef TUNACHAT_MESSAGECHANNEL_H
#define TUNACHAT_MESSAGECHANNEL_H

#include "ServerConn.h"
#include <thread>

using std::thread;

class MessageChannel {

    ServerConn &conn;
    thread th;
    int status = STATUS_OK;

public:

    explicit MessageChannel(ServerConn &conn);

    /**
     * Starts the MessageChannel run in the background after authentication to receive incoming data from the server.
     * The channel will continue to receive new data until the status has been set to STATUS_CLOSED.
     *
     * @return status code
     */
    int start();

    /**
     * Sends a signal to this message channel that the socket is being shutdown and should not receive any more data.
     *
     * @return status code
     */
    int shutdown();

    /**
     * Returns the ServerConn associated with this MessageChannel.
     *
     * @return ServerConn of channel
     */
    ServerConn& getConnection() const;

    /**
     * Returns the thread this message channel is running on.
     *
     * @return thread this is running on
     */
    thread& getThread() const;

    /**
     * Returns this MessageChannel's status code.
     *
     * @return status code
     */
    int getStatus() const;

};

#endif //TUNACHAT_MESSAGECHANNEL_H

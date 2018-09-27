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

    MessageChannel(ServerConn &conn);

    int start();

    ServerConn& getConnection() const;

    int getStatus() const;

};

#endif //TUNACHAT_MESSAGECHANNEL_H

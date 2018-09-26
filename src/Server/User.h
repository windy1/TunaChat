//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_USER_H
#define TUNACHAT_USER_H

#include "ChatServer.h"
#include <string>

using std::string;

class User {

    ChatServer &server;
    string name;
    mutable vector<ClientConnPtr> connections;

public:

    User(ChatServer &server, const string &name);

    /**
     * Attempts to send a message of the specified text from the specified username.
     *
     * @param from User that sent the message
     * @param text of message
     * @return the amount of connections the message was sent to
     */
    int sendMessage(const string &from, const string &text);

    /**
     * Returns all connections associated with this User.
     *
     * @return connections associated with user
     */
    const vector<ClientConnPtr>& getConnections() const;

    /**
     * Returns the server associated with this User.
     *
     * @return server instance
     */
    ChatServer& getServer() const;

    /**
     * Returns this User's name.
     *
     * @return user name
     */
    const string& getName() const;

};


#endif //TUNACHAT_USER_H

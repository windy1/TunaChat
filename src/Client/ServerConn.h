//
// Created by Walker Crouse on 9/26/18.
//

#ifndef TUNACHAT_SERVERCONN_H
#define TUNACHAT_SERVERCONN_H

#include "ChatClient.h"

class MessageChannel;

typedef shared_ptr<MessageChannel> MsgChanPtr;

class ServerConn {

    ChatClient &client;
    MsgChanPtr msgChan;
    string host;
    int port;
    int socket;
    int bufferSize;
    bool authenticated = false;
    string user;
    int status = STATUS_CLOSED;

    /**
     * Initializes the connection. Creates a new socket and connects to the host.
     *
     * @return status code
     */
    int init();

    /**
     * Sends the initial "HELLO" to the server and awaits the server's response.
     *
     * @return status code
     */
    int sayHello();

    /**
     * Sends the specified data to the server and assigns the response to res.
     *
     * @param req data being sent to server
     * @param res data being received from server
     */
    void getResponse(const string &req, string &res);

public:

    ServerConn(ChatClient &client, const string &host, int port, int bufferSize=1024);

    /**
     * Attempts to authenticate with the server using the specified username and password.
     *
     * @param user to authenticate as
     * @param pwd to authenticate with
     * @return status code
     */
    int authenticate(const string &user, const string &pwd);

    /**
     * Attempts to send a message to the specified user of the specified text.
     *
     * @param user to send message to
     * @param text of message
     * @return status code
     */
    int sendMessage(const string &user, const string &text);

    /**
     * Sends the server a request to print out the current user list. This user list will be received on the active
     * MessageChannel.
     *
     * @return status code
     */
    int requestList();

    /**
     * Returns the ChatClient associated with this connection.
     *
     * @return client instance
     */
    ChatClient& getClient() const;

    /**
     * Returns the currently active MessageChannel. If the client has not been authenticated, this will be null.
     *
     * @return currently active MessageChannel
     */
    MsgChanPtr getMessageChannel() const;

    /**
     * Returns true if this connection has been authenticated.
     *
     * @return true if authenticated
     */
    bool isAuthenticated() const;

    /**
     * Returns the currently authenticated user name, empty if none.
     *
     * @return user name of currently authenticated user
     */
    const string& getUser() const;

    /**
     * Returns the host of this connection.
     *
     * @return connection host
     */
    const string& getHost() const;

    /**
     * Returns the port the server is listening on.
     *
     * @return port number
     */
    int getPort() const;

    /**
     * Returns the socket this connection is using.
     *
     * @return socket number
     */
    int getSocket() const;

    /**
     * Returns the buffer size for incoming data.
     *
     * @return buffer size for incoming data
     */
    int getBufferSize() const;

    /**
     * Returns the current status code of this connection.
     *
     * @return current status code
     */
    int getStatus() const;

};

#endif //TUNACHAT_SERVERCONN_H

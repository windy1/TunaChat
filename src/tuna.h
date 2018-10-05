//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_TUNA_H
#define TUNACHAT_TUNA_H

///
/// == Statuses ==
///

#define STATUS_OK           200
#define STATUS_SHUTDOWN     201
#define STATUS_CLOSED       202
#define STATUS_BAD_SOCKET   500
#define STATUS_BAD_BIND     501
#define STATUS_BAD_LISTEN   502
#define STATUS_BAD_RESPONSE 503
#define STATUS_BAD_REQUEST  400
#define STATUS_NO_DATA      404
#define STATUS_UNKNOWN_CMD  401
#define STATUS_INVALID_ARG  402
#define STATUS_BAD_CONNECT  403
#define STATUS_TIMEOUT      408

///
/// == Protocol language ==
///

#define PROTO_HELLO     "HELLO"
#define PROTO_AUTHYES   "AUTHYES"
#define PROTO_AUTHNO    "AUTHNO"
#define PROTO_SIGNOFF   "SIGNOFF"
#define PROTO_AUTH      "AUTH"
#define PROTO_SIGNIN    "SIGNIN"
#define PROTO_LIST      "LIST"
#define PROTO_TO        "To"
#define PROTO_FROM      "From"
#define PROTO_BYE       "BYE"

///
/// == ncurses colors ==
///

#define COLOR_PAIR_DIVIDER  1
#define COLOR_PAIR_ERROR    2
#define COLOR_PAIR_TITLE    3

#include <string>
#include <vector>
#include <functional>

///
/// == Functions ==
///

namespace tuna {

    /**
     * Reads the next line from the specified socket. This function is blocking and will wait until there is data in the
     * buffer.
     *
     * @param data incoming data
     * @param socket to read from
     * @param bufferSize data buffer size
     * @return read size returned by recv
     */
    int readLine(std::string &data, int socket, int bufferSize);

    /**
     * Parses the specified string in the form of <header>:<body1>:<body2>.
     *
     * @param str string to parse
     * @param header portion
     * @param body1 portion
     * @param body2 portion
     * @return true if parse was successful
     */
    bool parse3(const std::string &str, std::string &header, std::string &body1, std::string &body2);

    /**
     * Parses the specified string in the form of <header>:<body>.
     *
     * @param str string to parse
     * @param header portion
     * @param body portion
     * @return true if parse was successful
     */
    bool parse2(const std::string &str, std::string &header, std::string &body);

    /**
     * Attempts to find the IP for the specified host name.
     *
     * @param name host name
     * @param ip result address
     * @return true if successful
     */
    bool host2ip(const std::string &name, std::string &ip);

    /**
     * Reads the file specified by the name line by line and calls the specified function for each line.
     *
     * @param fileName name of file to read
     * @param f function to call
     * @return true if opened file
     */
    bool fileFeed(const std::string &fileName, const std::function<void (const std::string &ln)> &f);

}

#endif //TUNACHAT_TUNA_H

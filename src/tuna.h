//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_TUNA_H
#define TUNACHAT_TUNA_H

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

#define COLOR_PAIR_DIVIDER  1
#define COLOR_PAIR_ERROR    2

#include <string>
#include <vector>
#include <functional>

namespace tuna {

    int readLine(std::string &data, int socket, int bufferSize);

    bool parse3(const std::string &str, std::string &header, std::string &body1, std::string &body2);

    bool parse2(const std::string &str, std::string &header, std::string &body);

    bool host2ip(const std::string &name, std::string &ip);

    bool fileFeed(const std::string &fileName, const std::function<void (const std::string &ln)> &f);

}

#endif //TUNACHAT_TUNA_H

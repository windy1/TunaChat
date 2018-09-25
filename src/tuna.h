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
#define STATUS_BAD_REQUEST  400
#define STATUS_NO_DATA      404

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

#endif //TUNACHAT_TUNA_H

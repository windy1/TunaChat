//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_TUNA_H
#define TUNACHAT_TUNA_H

#define ERR_NONE_TERM   201
#define ERR_NONE        200
#define ERR_SOCKET      500
#define ERR_BIND        501
#define ERR_LISTEN      502
#define ERR_NO_DATA     404
#define ERR_BAD_HELLO   400
#define ERR_AUTH_FORMAT 401
#define ERR_CMD_FORMAT  402

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

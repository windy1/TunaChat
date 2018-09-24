//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_TUNA_H
#define TUNACHAT_TUNA_H

#define ERR_NONE_TERM   -1
#define ERR_NONE        0
#define ERR_SOCKET      1
#define ERR_BIND        2
#define ERR_LISTEN      3
#define ERR_NO_DATA     4
#define ERR_BAD_HS      5
#define ERR_AUTH_FORMAT 6
#define ERR_CMD_FORMAT  7

#define PROTO_HELLO     "HELLO"
#define PROTO_AUTHYES   "AUTHYES"
#define PROTO_AUTHNO    "AUTHNO"
#define PROTO_SIGNOFF   "SIGNOFF"

#define PROTO_HEAD_AUTH     "AUTH"
#define PROTO_HEAD_SIGNIN   "SIGNIN"

#define PROTO_CMD_LIST  "LIST"
#define PROTO_CMD_TO    "To"
#define PROTO_CMD_FROM  "From"
#define PROTO_CMD_BYE   "BYE"

#endif //TUNACHAT_TUNA_H

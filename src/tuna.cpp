//
// Created by Walker Crouse on 9/26/18.
//

#include "tuna.h"
#include <sstream>
#include <sys/socket.h>

using std::string;
using std::stringstream;
using std::vector;

int readLine(string &data, int socket, int bufferSize) {
    char buffer[bufferSize];
    ssize_t readSize = recv(socket, buffer, (size_t) bufferSize, 0);
    stringstream in(buffer);
    in >> data;
    return (int) readSize;
}

bool parse3(const string &str, string &header, string &body1, string &body2) {
    unsigned long sep1 = str.find(':');
    unsigned long sep2 = str.rfind(':');
    if (sep1 == string::npos || sep2 == string::npos || sep1 == sep2) {
        return false;
    }

    header = str.substr(0, sep1);
    body1 = str.substr(sep1 + 1, sep2 - header.size() - 1);
    body2 = str.substr(sep2 + 1, string::npos);

    return true;
}

bool parse2(const string &str, string &header, string &body) {
    unsigned long sep = str.find(':');
    if (sep == string::npos || sep == 0 || sep == str.size() - 1) {
        return false;
    }

    header = str.substr(0, sep);
    body = str.substr(sep + 1, string::npos);

    return true;
}

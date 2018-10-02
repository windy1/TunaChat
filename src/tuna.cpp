//
// Created by Walker Crouse on 9/26/18.
//

#include "tuna.h"
#include <sstream>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fstream>

using std::string;
using std::stringstream;
using std::vector;
using std::function;
using std::ifstream;

int tuna::readLine(string &data, int socket, int bufferSize) {
    char buffer[bufferSize];
    ssize_t readSize = recv(socket, buffer, (size_t) bufferSize, 0);
//    stringstream in(buffer);
//    getline(in, data);
    data = buffer;
    data = data.substr(0, data.size() - 1);
    printf("buffer = %s\n", buffer);
    return (int) readSize;
}

bool tuna::parse3(const string &str, string &header, string &body1, string &body2) {
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

bool tuna::parse2(const string &str, string &header, string &body) {
    unsigned long sep = str.find(':');
    if (sep == string::npos || sep == 0 || sep == str.size() - 1) {
        return false;
    }

    header = str.substr(0, sep);
    body = str.substr(sep + 1, string::npos);

    return true;
}

bool tuna::host2ip(const string &name, string &ip) {
    hostent *host = gethostbyname(name.c_str());
    if (host == nullptr) return false;
    auto **addrList = (in_addr**) host->h_addr_list;
    if (addrList[0] != nullptr) {
        ip = string(inet_ntoa(*addrList[0]));
        return true;
    }
    return false;
}

bool tuna::fileFeed(const string &fileName, const function<void (const string &ln)> &f) {
    ifstream in(fileName);
    if (!in) return false;
    string ln;
    while (getline(in, ln)) f(ln);
    return true;
}

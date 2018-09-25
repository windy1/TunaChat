//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_USER_H
#define TUNACHAT_USER_H

#include <string>

using std::string;

class User {

    string name;

public:

    explicit User(const string &name);

    const string& getName() const;

};


#endif //TUNACHAT_USER_H

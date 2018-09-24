//
// Created by Walker Crouse on 9/24/18.
//

#ifndef TUNACHAT_USER_H
#define TUNACHAT_USER_H


#include <string>

class User {

    std::string name;

public:

    explicit User(const std::string &name);

    const std::string& getName() const;

};


#endif //TUNACHAT_USER_H

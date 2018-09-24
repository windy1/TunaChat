//
// Created by Walker Crouse on 9/24/18.
//

#include "User.h"

User::User(const std::string &name) : name(name) {}

const std::string& User::getName() const {
    return name;
}

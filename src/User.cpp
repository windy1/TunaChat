//
// Created by Walker Crouse on 9/24/18.
//

#include "User.h"

User::User(const string &name) : name(name) {}

const string& User::getName() const {
    return name;
}

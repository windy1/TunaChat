//
// Created by Walker Crouse on 10/5/18.
//

#ifndef TUNACHAT_PREFERENCES_H
#define TUNACHAT_PREFERENCES_H

#include "Terminal/Terminal.h"
#include <map>
#include <string>
#include <iostream>

using std::map;
using std::string;
using std::ostream;

class Preferences {

    mutable map<string, string> prefs;
    string fileName;

public:

    static const string nval;

    bool save();

    bool keyExists(const string &key) const;

    const string& operator()(const string &key) const;

    bool operator()(const string &key, const string &value) const;

    int getInt(const string &key) const;

    int getBool(const string &key) const;

    const string& getFileName() const;

    static bool load(Preferences &prefs, const string &fileName);

    friend string to_string(const Preferences &prefs);

};

#endif //TUNACHAT_PREFERENCES_H

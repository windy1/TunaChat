//
// Created by Walker Crouse on 10/5/18.
//

#include "Preferences.h"
#include <fstream>

using std::ofstream;
using std::ifstream;
using std::endl;

///
/// == Statics ==
///

const string Preferences::nval = string();

///
/// == Methods ==
///

bool Preferences::save() {
    ofstream out(fileName);
    if (!out) return false;
    for (auto &pair : prefs) {
        out << pair.first << '=' << pair.second << endl;
    }
    out.close();
    return true;
}

bool Preferences::keyExists(const string &key) const {
    return prefs.find(key) != prefs.end();
}

///
/// == Key lookups ==
///

const string& Preferences::operator()(const string &key) const {
    return keyExists(key) ? prefs[key] : nval;
}

bool Preferences::operator()(const string &key, const string &value) const {
    if (keyExists(key)) {
        prefs[key] = value;
        return true;
    }
    return false;
}

int Preferences::getInt(const string &key) const {
    try {
        return stoi((*this)(key));
    } catch (...) {
        return -1;
    }
}

int Preferences::getBool(const string &key) const {
    return (bool) getInt(key);
}

///
/// == Getters ==
///

const string& Preferences::getFileName() const {
    return fileName;
}

///
/// == Static methods ==
///

bool Preferences::load(Preferences &prefs, const string &fileName) {
    ifstream in(fileName);
    if (!in) return false;
    prefs.fileName = fileName;
    string pair;
    while (in >> pair) {
        size_t sep = pair.find('=');
        string key = pair.substr(0, sep);
        string value = pair.substr(sep + 1);
        prefs.prefs[key] = value;
        in.ignore();
    }
    return true;
}

string to_string(const Preferences &prefs) {
    string str = "* Preferences\n";
    for (auto &pair : prefs.prefs) {
        str += "\t" + pair.first + '=' + pair.second + '\n';
    }
    return str;
}

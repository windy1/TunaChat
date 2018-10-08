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

    /**
     * String reference that is returned when there is no preference that exists for a key.
     */
    static const string nval;

    /**
     * Saves these preferences to disk.
     *
     * @return true if successful
     */
    bool save();

    /**
     * Returns true if the specified key exists in the preferences.
     *
     * @param key key to check for
     * @return true if key exists
     */
    bool keyExists(const string &key) const;

    /**
     * Returns the string value for the specified key if it exists.
     *
     * @param key to get value for
     * @return
     */
    const string& operator()(const string &key) const;

    /**
     * Sets the specified key to the given value if the key exists.
     *
     * @param key to set value for
     * @param value to set key to
     * @return true if value was set
     */
    bool operator()(const string &key, const string &value) const;

    /**
     * Returns the specified preference as an integer.
     *
     * @param key to get value for
     * @return integer preference if found, -1 otherwise
     */
    int getInt(const string &key) const;

    /**
     * Returns the specifie preference as a boolean.
     *
     * @param key to get value for
     * @return bool preference if found
     */
    int getBool(const string &key) const;

    /**
     * Returns the name of the file this is saved to.
     *
     * @return Preference file name
     */
    const string& getFileName() const;

    /**
     * Loads preferences from the specified file into the given Preferences reference.
     *
     * @param prefs to load into
     * @param fileName preference file
     * @return true if successful
     */
    static bool load(Preferences &prefs, const string &fileName);

    /// std::to_string overload
    friend string to_string(const Preferences &prefs);

};

#endif //TUNACHAT_PREFERENCES_H

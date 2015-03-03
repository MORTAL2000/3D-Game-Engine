#ifndef ACTION_LISTENER_H
#define ACTION_LISTENER_H

#include <map>

typedef std::map<char, int> KeyMap;

class ActionListener
{
public:
    ActionListener();

    int& operator[](char);
    int& operator[](int);
private:
    KeyMap m_map;
};

#endif

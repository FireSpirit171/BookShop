#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class SETTINGS {
public:
    static QString get_host_name();
    static int get_port();
    static QString get_database_name();
    static QString get_user_name();
    static QString get_password();
};

#endif // SETTINGS_H

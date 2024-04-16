#include "SETTINGS.h"

QString SETTINGS::get_host_name(){
    return "localhost";
}

int SETTINGS::get_port(){
    return 5432;
}

QString SETTINGS::get_database_name(){
    return "BookShop";
}

QString SETTINGS::get_user_name(){
    return "postgres";
}

QString SETTINGS::get_password(){
    return "VIRM21092804";
}

#ifndef USER_H
#define USER_H

#include "../chat_config.h"

struct User {
    char name[NAMELENGTH];
    char password[NAMELENGTH];
    int in_use;
    int logged_in;
};

extern struct User users[MAXUSERS];

void init_users(void);
int create_user(const char *name, const char *password);
int login_user(const char *name, const char *password);
int logout_user(const char *name);
int find_user(const char *name);

#endif

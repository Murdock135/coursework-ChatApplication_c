#include <stdio.h>
#include <string.h>
#include "chat_config.h"
#include "server/user.h"

// Global user database
struct User users[MAXUSERS];

// Initialize users array
void init_users(void) {
    for (int i = 0; i < MAXUSERS; i++) {
        users[i].in_use = 0;
        users[i].logged_in = 0;
        memset(users[i].name, 0, NAMELENGTH);
        memset(users[i].password, 0, NAMELENGTH);
    }
}

// Find a user by name, return index or -1 if not found
int find_user(const char *name) {
    for (int i = 0; i < MAXUSERS; i++) {
        if (users[i].in_use && strcmp(users[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Create a new user account
// Returns: 1 if success, 0 if user already exists, -1 if no space
int create_user(const char *name, const char *password) {
    // Check if user already exists
    if (find_user(name) != -1) {
        return 0;  // User already exists
    }
    
    // Find empty slot
    for (int i = 0; i < MAXUSERS; i++) {
        if (!users[i].in_use) {
            strncpy(users[i].name, name, NAMELENGTH - 1);
            strncpy(users[i].password, password, NAMELENGTH - 1);
            users[i].in_use = 1;
            users[i].logged_in = 0;
            return 1;  // Success
        }
    }
    
    return -1;  // No space available
}

// Login a user
// Returns: 1 if success, 0 if credentials wrong or user doesn't exist, -1 if already logged in
int login_user(const char *name, const char *password) {
    int idx = find_user(name);
    
    // User doesn't exist
    if (idx == -1) {
        return 0;
    }
    
    // User already logged in
    if (users[idx].logged_in) {
        return -1;
    }
    
    // Check password
    if (strcmp(users[idx].password, password) != 0) {
        return 0;  // Wrong password
    }
    
    // Login successful
    users[idx].logged_in = 1;
    return 1;
}

// Logout a user
// Returns: 1 if success, 0 if user not found or not logged in
int logout_user(const char *name) {
    int idx = find_user(name);
    
    if (idx == -1) {
        return 0;  // User not found
    }
    
    if (!users[idx].logged_in) {
        return 0;  // Not logged in
    }
    
    users[idx].logged_in = 0;
    return 1;  // Success
}

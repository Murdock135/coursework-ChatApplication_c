#include <stdio.h>
#include <string.h>
#include "chat_config.h"
#include "server/user.h"

// Global user database
struct User users[MAXUSERS];

// Save all users to file
static void save_users(void) {
    FILE *f = fopen(USERS_FILE, "w");
    if (f == NULL) {
        fprintf(stderr, "Warning: could not save users to %s\n", USERS_FILE);
        return;
    }
    for (int i = 0; i < MAXUSERS; i++) {
        if (users[i].in_use) {
            fprintf(f, "(%s, %s)\n", users[i].name, users[i].password);
        }
    }
    fclose(f);
}

// Initialize users array and load from file
void init_users(void) {

    // Start with no users
    for (int i = 0; i < MAXUSERS; i++) {
        users[i].in_use = 0;
        users[i].logged_in = 0;
        memset(users[i].name, 0, NAMELENGTH);
        memset(users[i].password, 0, NAMELENGTH);
    }

    // Load users from file
    FILE *f = fopen(USERS_FILE, "r");
    if (f == NULL) return;  // No file yet, start fresh

    int slot = 0;
    char name[NAMELENGTH], password[NAMELENGTH];
    while (slot < MAXUSERS && fscanf(f, " (%31[^,], %31[^)])\n", name, password) == 2) {
        strncpy(users[slot].name, name, NAMELENGTH - 1);
        strncpy(users[slot].password, password, NAMELENGTH - 1);
        users[slot].in_use = 1;
        users[slot].logged_in = 0;
        slot++;
    }
    fclose(f);
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
// Returns: 1 if success, 0 if user already exists, -1 if no space, -2 if invalid lengths
int create_user(const char *name, const char *password) {
    // Validate lengths
    int name_len = strlen(name);
    int pass_len = strlen(password);
    if (name_len < USERNAME_MIN || name_len > USERNAME_MAX) return -2;
    if (pass_len < PASSWORD_MIN || pass_len > PASSWORD_MAX) return -2;

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
            save_users();
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

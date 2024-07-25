#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_PATH_LENGTH 32767

void modifyPathVariable(const char *path, int remove) {
    HKEY hKey;
    LONG result;
    char currentPath[MAX_PATH_LENGTH];
    DWORD bufferSize = sizeof(currentPath);

    // Open the registry key with write permissions
    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_SET_VALUE, &hKey);
    if (result != ERROR_SUCCESS) {
        fprintf(stderr, "Failed to open registry key. Error code: %ld\n", result);
        return;
    }

    // Query the current PATH value
    result = RegQueryValueEx(hKey, "Path", NULL, NULL, (LPBYTE)currentPath, &bufferSize);
    if (result != ERROR_SUCCESS && result != ERROR_FILE_NOT_FOUND) {
        fprintf(stderr, "Failed to query registry value. Error code: %ld\n", result);
        RegCloseKey(hKey);
        return;
    }

    // If the PATH value is not found, initialize it as empty
    if (result == ERROR_FILE_NOT_FOUND) {
        currentPath[0] = '\0';
    }

    // Prepare the new PATH value
    char newPath[MAX_PATH_LENGTH];
    if (remove) {
        // Remove the specified path
        char *start = strstr(currentPath, path);
        if (start) {
            char *end = start + strlen(path);
            if (*end == ';') end++;
            memmove(start, end, strlen(end) + 1);
        }
        // Remove trailing semicolon if it exists
        if (strlen(currentPath) > 0 && currentPath[strlen(currentPath) - 1] == ';') {
            currentPath[strlen(currentPath) - 1] = '\0';
        }
    } else {
        // Append the new path if it's not already present
        if (strstr(currentPath, path) == NULL) {
            if (strlen(currentPath) > 0 && currentPath[strlen(currentPath) - 1] != ';') {
                strcat(currentPath, ";");
            }
            strcat(currentPath, path);
        }
    }

    // Set the new PATH value
    result = RegSetValueEx(hKey, "Path", 0, REG_EXPAND_SZ, (const BYTE *)currentPath, strlen(currentPath) + 1);
    if (result != ERROR_SUCCESS) {
        fprintf(stderr, "Failed to set registry value. Error code: %ld\n", result);
    } else {
        printf("PATH modified successfully.\n");
    }

    RegCloseKey(hKey);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <path> [--remove]\n", argv[0]);
        return 1;
    }

    int remove = 0;
    if (argc > 2 && strcmp(argv[2], "--remove") == 0) {
        remove = 1;
    }

    modifyPathVariable(argv[1], remove);

    return 0;
}

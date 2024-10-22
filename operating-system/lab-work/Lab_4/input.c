#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void print_date_time() 
{
    time_t now;
    struct tm *tm_info;
    char buffer[26];
    time(&now);
    tm_info = localtime(&now);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Current date and time: %s\n", buffer);
}

int main() 
{
    char username[256];

    if (getlogin_r(username, sizeof(username)) != 0) 
    {
        perror("getlogin_r");
        return EXIT_FAILURE;
    }

    printf("Welcome, %s!\n", username);
    print_date_time();
    return EXIT_SUCCESS;
}


// ME: A QUICK PROGRAM TO CHECK THAT I DIDN'T MISTAKENLY BREAK ANYTHING IN PITT TESTS
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
using namespace std;

bool validate_test_file(char *filename)
{
    bool stat = false;
    size_t j{};
    for (; j < strlen(filename); j++)
    {
        if (filename[j] == '.' && filename[j - 1] != '/')
        {
            break;
        }
    }
    char buffer[strlen(filename) - (j + 1)] = {'\0'};
    uint8_t buf_c = 0;
    j++;
    for (; j < strlen(filename); j++)
    {
        buffer[buf_c++] = filename[j];
    }
    buffer[buf_c++] = '\0';
    if (strcmp(buffer, "pitt") == 0)
    {
        stat = true;
    }
    return stat;
}

int main(void)
{
    DIR *testdir = nullptr;
    struct dirent *entry = nullptr;

    testdir = opendir("Tests");
    if (testdir)
    {
        while ((entry = readdir(testdir)) != NULL)
        {   
            if (validate_test_file(entry->d_name))
            {
                cout << endl << "[CMD] Target File: " << entry->d_name << endl;
                char buffer[128];
                sprintf(buffer,"pitt build ./Tests/%s",entry->d_name);
                system(buffer);
            }
        }
        closedir(testdir);
    }

    return 0;
}
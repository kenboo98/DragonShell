#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "builtin.h"

/**
 * @brief Tokenize a string 
 * 
 * @param str - The string to tokenize
 * @param delim - The string containing delimiter character(s)
 * @return std::vector<std::string> - The list of tokenized strings. Can be empty
 */
using namespace std;

std::vector<std::string> tokenize(const std::string &str, const char *delim) {
    char *cstr = new char[str.size() + 1];
    std::strcpy(cstr, str.c_str());

    char *tokenized_string = strtok(cstr, delim);

    std::vector<std::string> tokens;
    while (tokenized_string != NULL) {
        tokens.push_back(std::string(tokenized_string));
        tokenized_string = strtok(NULL, delim);
    }
    delete[] cstr;

    return tokens;
}
vector<char*> stringToCharVector(vector<string> &original){
    vector<char*> charVector;
    for (string s: original){
        char* constS = &s[0];
        charVector.push_back(constS);
    }
    return charVector;
}
void printWelcome() {
    const char *welcome =
            "\n"
            "                        \\`-\\`-._\n"
            "                         \\` )`. `-.__      ,\n"
            "      '' , . _       _,-._;'_,-`__,-'    ,/\n"
            "     : `. ` , _' :- '--'._ ' `------._,-;'\n"
            "      `- ,`- '            `--..__,,---'   \n"
            ""
            "               Welcome to Dragon Shell!     \n";

    cout << welcome;
}

bool builtInCommands(vector<string> &tokens, vector<string> &paths) {
    if (tokens[0] == "pwd") {
        pwd();
        return true;
    } else if (tokens[0] == "cd") {
        cd(tokens);
        return true;
    } else if (tokens[0] == "$PATH") {
        printPath(paths);
        return true;
    } else if (tokens[0] == "a2path") {
        vector<string> newPaths;
        newPaths = tokenize(tokens[1], ":");
        a2path(newPaths, paths);
        return true;
    } else if (tokens[0] == "exit"){
        _exit(1);
    }
    return false;
}

int main(int argc, char **argv) {
    // print the string prompt without a newline, before beginning to read
    // tokenize the input, run the command(s), and print the result
    // do this in a loop
    printWelcome();
    vector<string> paths = {"/usr/bin/", "/bin/"};

    while (true) {
        cout << "dragonshell >";
        string input;
        getline(cin, input);
        vector<string> commands;
        commands = tokenize(input, ";");
        for (string command: commands) {
            vector<string> tokens;
            tokens = tokenize(command, " ");
            int parentPid = getpid();
            if (builtInCommands(tokens, paths)) {

            } else {
                int rc = fork();
                if (rc == 0) {
                    int result;
                    //child process
                    vector<char*> charTokens = stringToCharVector(tokens);
                    execve(tokens[0].c_str(), &charTokens[0], environ);
                    return result;
                } else {
                    //parent process
                    int w_status;
                    wait(&w_status);
                    if (WIFEXITED(w_status) != 1) {
                        cout << "Error" << "\n";
                    }
                }
            }
        }


    }


}
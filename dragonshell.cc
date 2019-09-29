#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "builtin.h"
#include <algorithm>
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

void removeSpaces(string &s) {
    string::iterator endPos = remove(s.begin(), s.end(), ' ');
    s.erase(endPos, s.end());
}

vector<char *> stringToCharVector(vector<string> &original) {
    vector<char *> charVector;
    for (string s: original) {
        char *cstr = new char[s.size() + 1];
        std::strcpy(cstr, s.c_str());
        char *constS = &cstr[0];
        charVector.push_back(constS);
    }
    return charVector;
}

void exec(vector<string> tokens, vector<string> &paths) {

    vector<char *> charTokens = stringToCharVector(tokens);
    execve(tokens[0].c_str(), &charTokens[0], environ);

    for (string path: paths) {
        vector<string> temp(tokens);
        temp[0] = path + temp[0];
        charTokens = stringToCharVector(temp);
        execve(temp[0].c_str(), &charTokens[0], environ);
    }
    cout << "Command not found" << "\n";
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
    } else if (tokens[0] == "exit") {
        _exit(1);
    }
    return false;
}

int outputRedirection(vector<string> &redirectCommands, vector<string> &paths) {

    int rc = fork();
    if (rc == 0) {
        int file_descriptor;
        removeSpaces(redirectCommands[1]);
        string filename = redirectCommands[1];
        if ((file_descriptor = open(filename.c_str(), O_CREAT | O_WRONLY, 0644)) < 0)
            perror("open failed!");
        dup2(file_descriptor, STDOUT_FILENO);    // stdin = ffd
        close(file_descriptor);
        vector<string> tokens = tokenize(redirectCommands[0], " ");
        exec(tokens, paths);
    } else {
        //parent process
        int w_status;
        wait(&w_status);
        if (WIFEXITED(w_status) != 1) {
            cout << "Error" << "\n";
        }
    }

    return 0;
}


int pipePrograms(vector<string> &pipedPrograms, vector<string> &paths) {
    int fd[2];
    if (pipe(fd) < 0)
        perror("Piping Error");
    vector<string> tokens;
    int rc = fork();
    if (rc == 0) {
        dup2(fd[1], STDOUT_FILENO);	// stdout = fd[1]
        close(fd[1]); 				// stdout is still open
        close(fd[0]);
        tokens = tokenize(pipedPrograms[0], " ");
        exec(tokens, paths);

    } else {
        //fork the next process
        int w_status;
        wait(&w_status);
        int rc = fork();
        if (rc == 0){
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            tokens = tokenize(pipedPrograms[1], " ");
            exec(tokens, paths);
        } else {
            close(fd[0]);
            close(fd[1]);
            int w_status;
            wait(&w_status);
        }

    }


    return 0;
}

int singleExternalProgram(vector<string> &tokens, vector<string> &paths) {
    int rc = fork();
    if (rc == 0) {
        int result = 0;
        //child process
        exec(tokens, paths);
        return result;
    } else {
        //parent process
        int w_status;
        wait(&w_status);
        if (WIFEXITED(w_status) != 1) {
            cout << "Error" << "\n";
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    // print the string prompt without a newline, before beginning to read
    // tokenize the input, run the command(s), and print the result
    // do this in a loop
    printWelcome();
    vector<string> paths = {"/usr/bin/", "/bin/"};

    while (true) {
        cout << "dragonshell > ";
        string input;
        getline(cin, input);

        vector<string> commands;
        commands = tokenize(input, ";");
        for (string command: commands) {

            vector<string> redirects;
            vector<string> piped;
            redirects = tokenize(command, ">");
            piped = tokenize(command, "|");
            if (redirects.size() > 1) {
                outputRedirection(redirects, paths);
            } else if (piped.size() > 1 ) {
                pipePrograms(piped, paths);
            } else {
                vector<string> tokens;
                tokens = tokenize(command, " ");
                int parentPid = getpid();
                if (builtInCommands(tokens, paths)) {

                } else {
                    singleExternalProgram(tokens, paths);
                }

            }

        }
    }

}
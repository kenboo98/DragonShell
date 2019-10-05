#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "builtin.h"
#include <algorithm>
#include <signal.h>

using namespace std;

std::vector<std::string> tokenize(const std::string &str, const char *delim) {
    // Tokenize function provided
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
    // Simple function to remove spaces from a string
    string::iterator endIterator = remove(s.begin(), s.end(), ' ');
    s.erase(endIterator, s.end());
}

vector<char *> stringToCharVector(vector<string> &original) {
    // Convert a string vector to a char vector to pass into the execve function
    vector<char *> charVector;
    for (string s: original) {
        char *cstr = new char[s.size() + 1];
        std::strcpy(cstr, s.c_str());
        char *constS = &cstr[0];
        charVector.push_back(constS);
    }
    return charVector;
}

void exec(vector<string> &tokens, vector<string> &dPaths) {
    // Wrapper function to execute a program using the execve
    vector<char *> charTokens = stringToCharVector(tokens);
    execve(tokens[0].c_str(), &charTokens[0], environ);

    for (const auto &path: dPaths) {
        vector<string> temp(tokens);
        temp[0] = path + temp[0];
        charTokens = stringToCharVector(temp);
        execve(temp[0].c_str(), &charTokens[0], environ);
    }
    cout << "Command not found" << "\n";
}

void setChildHandlers() {
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
}

void setSignalHandlers() {
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
}

void printWelcome() {
    // Ascii Art taken from http://ascii.co.uk/art/dragon
    const char *welcome =
            "                                        ,   ,\n"
            "                                        $,  $,     ,\n"
            "                                        \"ss.$ss. .s'\n"
            "                                ,     .ss$$$$$$$$$$s,\n"
            "                                $. s$$$$$$$$$$$$$$`$$Ss\n"
            "                                \"$$$$$$$$$$$$$$$$$$o$$$       ,\n"
            "                               s$$$$$$$$$$$$$$$$$$$$$$$$s,  ,s\n"
            "                              s$$$$$$$$$\"$$$$$$\"\"\"\"$$$$$$\"$$$$$,\n"
            "                              s$$$$$$$$$$s\"\"$$$$ssssss\"$$$$$$$$\"\n"
            "                             s$$$$$$$$$$'         `\"\"\"ss\"$\"$s\"\"\n"
            "                             s$$$$$$$$$$,              `\"\"\"\"\"$  .s$$s\n"
            "                             s$$$$$$$$$$$$s,...               `s$$'  `\n"
            "                         `ssss$$$$$$$$$$$$$$$$$$$$####s.     .$$\"$.   , s-\n"
            "                           `\"\"\"\"$$$$$$$$$$$$$$$$$$$$#####$$$$$$\"     $.$'\n"
            "                                 \"$$$$$$$$$$$$$$$$$$$$$####s\"\"     .$$$|\n"
            "                                  \"$$$$$$$$$$$$$$$$$$$$$$$$##s    .$$\" $\n"
            "                                   $$\"\"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\"   `\n"
            "                                  $$\"  \"$\"$$$$$$$$$$$$$$$$$$$$S\"\"\"\"'\n"
            "                             ,   ,\"     '  $$$$$$$$$$$$$$$$####s\n"
            "                             $.          .s$$$$$$$$$$$$$$$$$####\"\n"
            "                 ,           \"$s.   ..ssS$$$$$$$$$$$$$$$$$$$####\"\n"
            "                 $           .$$$S$$$$$$$$$$$$$$$$$$$$$$$$#####\"\n"
            "                 Ss     ..sS$$$$$$$$$$$$$$$$$$$$$$$$$$$######\"\"\n"
            "                  \"$$sS$$$$$$$$$$$$$$$$$$$$$$$$$$$########\"\n"
            "           ,      s$$$$$$$$$$$$$$$$$$$$$$$$#########\"\"'\n"
            "           $    s$$$$$$$$$$$$$$$$$$$$$#######\"\"'      s'         ,\n"
            "           $$..$$$$$$$$$$$$$$$$$$######\"'       ....,$$....    ,$\n"
            "            \"$$$$$$$$$$$$$$$######\"' ,     .sS$$$$$$$$$$$$$$$$s$$\n"
            "              $$$$$$$$$$$$#####\"     $, .s$$$$$$$$$$$$$$$$$$$$$$$$s.\n"
            "   )          $$$$$$$$$$$#####'      `$$$$$$$$$###########$$$$$$$$$$$.\n"
            "  ((          $$$$$$$$$$$#####       $$$$$$$$###\"       \"####$$$$$$$$$$\n"
            "  ) \\         $$$$$$$$$$$$####.     $$$$$$###\"             \"###$$$$$$$$$   s'\n"
            " (   )        $$$$$$$$$$$$$####.   $$$$$###\"                ####$$$$$$$$s$$'\n"
            " )  ( (       $$\"$$$$$$$$$$$#####.$$$$$###' -Tua Xiong     .###$$$$$$$$$$\"\n"
            " (  )  )   _,$\"   $$$$$$$$$$$$######.$$##'                .###$$$$$$$$$$\n"
            " ) (  ( \\.         \"$$$$$$$$$$$$$#######,,,.          ..####$$$$$$$$$$$\"\n"
            "(   )$ )  )        ,$$$$$$$$$$$$$$$$$$####################$$$$$$$$$$$\"\n"
            "(   ($$  ( \\     _sS\"  `\"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$S$$,\n"
            " )  )$$$s ) )  .      .   `$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\"'  `$$\n"
            "  (   $$$Ss/  .$,    .$,,s$$$$$$##S$$$$$$$$$$$$$$$$$$$$$$$$S\"\"        '\n"
            "    \\)_$$$$$$$$$$$$$$$$$$$$$$$##\"  $$        `$$.        `$$.\n"
            "        `\"S$$$$$$$$$$$$$$$$$#\"      $          `$          `$\n"
            "            `\"\"\"\"\"\"\"\"\"\"\"\"\"'         '           '           '\n";

    cout << welcome;
}

bool builtInCommands(vector<string> &tokens, vector<string> &dPaths, int &background) {
    // checks through built in commands
    if (tokens[0] == "pwd") {
        pwd();
        return true;
    } else if (tokens[0] == "cd") {
        cd(tokens);
        return true;
    } else if (tokens[0] == "$PATH") {
        printPath(dPaths);
        return true;
    } else if (tokens[0] == "a2path") {
        vector<string> newPaths;
        newPaths = tokenize(tokens[1], ":");
        a2path(newPaths, dPaths);
        return true;
    } else if (tokens[0] == "exit") {
        dsExit(background);
    }
    return false;
}

int outputRedirection(vector<string> &redirectCommands, vector<string> &dPaths) {
    // Function to do Output Redirection
    pid_t rc = fork();
    if (rc == 0) {
        int file_descriptor;
        removeSpaces(redirectCommands[1]);
        string filename = redirectCommands[1];
        if ((file_descriptor = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644)) < 0)
            perror("open failed!");
        dup2(file_descriptor, STDOUT_FILENO);    // stdin = ffd
        close(file_descriptor);
        vector<string> tokens = tokenize(redirectCommands[0], " ");
        exec(tokens, dPaths);
    } else {
        //parent process
        int w_status;
        waitpid(rc, &w_status, 0);
        if (WIFEXITED(w_status) != 1) {
            cout << "Error" << "\n";
        }
    }

    return 0;
}


int pipePrograms(vector<string> &pipedPrograms, vector<string> &dPaths) {
    // Function to accomplish piping from one program to another.
    int fd[2];
    if (pipe(fd) < 0)
        perror("Piping Error");
    vector<string> tokens;
    pid_t rc = fork();
    if (rc == 0) {
        setChildHandlers();
        dup2(fd[1], STDOUT_FILENO);    // stdout = fd[1]
        close(fd[1]);                // stdout is still open
        close(fd[0]);
        tokens = tokenize(pipedPrograms[0], " ");
        exec(tokens, dPaths);

    } else {
        //fork the next process
        int w_status;
        waitpid(rc, &w_status, 0);
        pid_t rc = fork();
        if (rc == 0) {
            setChildHandlers();
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            tokens = tokenize(pipedPrograms[1], " ");
            exec(tokens, dPaths);
        } else {
            close(fd[0]);
            close(fd[1]);
            int w_status;
            waitpid(rc, &w_status, 0);
        }

    }


    return 0;
}

int backgroundProgram(vector<string> &tokens, vector<string> &dPaths, int &background) {
    // Function to run background program
    tokens.pop_back();

    int rc = fork();
    if (rc == 0) {
        setChildHandlers();
        background = rc;
        int devnull;
        if ((devnull = open("/dev/null", O_WRONLY, 0644)) < 0)
            perror("Could not write to devnull!");
        dup2(devnull, STDOUT_FILENO);    // stdin = ffd
        close(devnull);
        exec(tokens, dPaths);
        return 0;
    } else {
        cout << "PID " << rc << " is running in the background" << "\n";
    }
    return 0;
}

int singleExternalProgram(vector<string> &tokens, vector<string> &dPaths) {
    pid_t rc = fork();

    if (rc == 0) {
        setChildHandlers();
        int result = 0;
        //child process
        exec(tokens, dPaths);
        return result;
    } else {
        //parent process
        int w_status;
        waitpid(rc, &w_status, 0);
    }
    return 0;
}


int main(int argc, char **argv) {
    // Main loop that runs continuously.
    printWelcome();
    vector<string> dPaths = {"/usr/bin/", "/bin/"};
    setSignalHandlers();
    // store the PID of the background task.
    int background = -1;
    while (true) {
        cout << "dragonshell > ";
        string input;
        if (!getline(cin, input)) {
            cout << "\n";
            dsExit(background);
        }

        vector<string> commands;
        commands = tokenize(input, ";");
        for (string command: commands) {

            vector<string> redirects;
            vector<string> piped;
            redirects = tokenize(command, ">");
            piped = tokenize(command, "|");
            if (redirects.size() > 1) {
                outputRedirection(redirects, dPaths);
            } else if (piped.size() > 1) {
                pipePrograms(piped, dPaths);
            } else {
                vector<string> tokens;
                tokens = tokenize(command, " ");
                if (builtInCommands(tokens, dPaths, background)) {

                } else {
                    if (tokens[tokens.size() - 1] == "&") {
                        backgroundProgram(tokens, dPaths, background);
                    } else {
                        singleExternalProgram(tokens, dPaths);
                    }
                }

            }

        }
    }

}
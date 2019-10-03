//
// Created by ken on 2019-09-25.
//
#include <vector>
#include <string>
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <errno.h>

using namespace std;


int pwd() {
    string cwd = get_current_dir_name();
    cout << cwd << "\n";
    return 1;
}

int cd(vector<string> &tokens) {

    int result = chdir(tokens[1].c_str());
    if (result != 1) {
        switch (errno) {
            case ENOENT:
                cout << "No such file or directory" << "\n";
            case EACCES:
                cout << "Permission denied" << "\n";
            case EFAULT:
                cout << "Expected Argument to \"cd\"" << "\n";
        }
    }
    return result;
}

void printPath(vector<string> &paths) {
    cout << "Current PATH: ";
    for (vector<string>::size_type i = 0; i != paths.size(); i++) {
        cout << paths[i];
        if (i != paths.size() - 1) {
            cout << ":";
        }
    }
    cout << "\n";
}

void a2path(vector<string> &newPaths, vector<string> &paths) {
    if (newPaths[0] == "$PATH") {
        for (vector<string>::size_type i = 1; i != newPaths.size(); i++) {
            paths.push_back(newPaths[i]);
        }
    } else {
        paths.swap(newPaths);
    }

}
int dsExit(int &background){
    if (background != -1) {
        kill(background, SIGHUP);
    }
    _exit(1);
}
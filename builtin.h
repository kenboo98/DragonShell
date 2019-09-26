//
// Created by ken on 2019-09-25.
//

#ifndef A1_BUILTIN_H
#define A1_BUILTIN_H

#endif //A1_BUILTIN_H
#include <vector>
#include <string>

using namespace std;

int pwd();
int cd(vector<string> &tokens);
int printPath(vector<string> &paths);
int a2path(vector<string> &newPaths, vector<string> &paths);
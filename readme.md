# DragonShell implimentations by Kenta Tellambura
### Running DragonShell
Go into your dragonshell directory and run the following commmands.
Requires g++ to compile.
```
make
./dragonshell
```
 ### System Calls
 Below are all the syscalls used to make DragonShell work properly.
 #### wait_pid
This system call is used to block Dragonshell to wait for a process to complete. We use `wait_pid` instead of `wait` to select a particular process to wait on. This is to ensure that in the event that there is a program running in the background, `wait` isn't unblocked by the wrong process.
 #### execve
 This is one of the most crucial system calls in Dragon shell. This allows for the execution of an external program. In DragonShell, I created a wrapper for `execve` to go through all paths in the `$PATH` variable when running an external program. Since `execve` completely replaces the current program with the external program, this is only run in a child process that would be created with the system call below. 
 #### fork
 This system call is used to fork a child process. The child is identical to the parent with the exception of the return value of `fork`. This is always called before running a program with DragonShell with the exception of builtin functions that were implemented by me. Builtin functions cannot be run in a child process since the changes made in the child process would not be mirrored in the parent. When `fork` is called, Dragonshell would wait for the process unless it was meant to be run in the background.
 #### pipe
 This is called to implement the piping features in DragonShell. The `pipe` system call is used to create a new pipe between the parent and child. To implement the piping features, we pipe the STANDARD_OUT of one program into the pipe from there, we pipe the result into the STANDARD_IN of the other program. To redirect filehanders to different pipe ends, we use the following systemcall. 
 #### dup2
 `dup2` is used often to redirect output to different targets. For piping, we use `dup2` to redirect STANDARD_OUT to one pipe end and redirect another pipe end to STANDARD_IN of another program. `dup2` is also used write STD_OUT of a program to a file or in the case of background processes, `/dev/null`.
 #### sigaction
 `sigaction` is used to register signal handlers. We set these handlers before the main loop in DragonShell. We create handlers for `SIGINT` and `SIGTSTIP` to ensure that Ctrl-Z and Ctrl-C do exit the program back to bash.
 #### chdir
 This function allows us to change the working directory of DragonShell when a user executes cd.
 
 ## Design and Implementation
 When DragonShell is first initialized, a welcome message is printed on the screen, all the paths are initialized, and the signal handlers are set as well. Then, we see the main loop where most of the work is done. On every iteration, the user's input is parsed. First, the input is split by the `;` delimiter to split it into individual commands. Then, the command is split by the `>` and `|` delimiter
 to check for any piping or output redirection. Depending on the result of the above tokenizeations, the commands are sent to individual functions to handle different cases. 
 ## Tests
 Some examples of tests run can be seen below:
 ```
dragonshell > ls
builtin.cc  builtin.h  builtin.o  cmake-build-debug  CMakeLists.txt  dragonshell  dragonshell.cc  dragonshell.o  Makefile  readme.md
dragonshell > cd ..
dragonshell > ls
a1  a1-starter-code
dragonshell > cd a1	
dragonshell > ls
builtin.cc  builtin.h  builtin.o  cmake-build-debug  CMakeLists.txt  dragonshell  dragonshell.cc  dragonshell.o  Makefile  readme.md
dragonshell > find ./|head -n 5 
./
./.idea
./.idea/a1.iml
./.idea/workspace.xml
./.idea/modules.xml
dragonshell > ls > test.txt; cat test.txt|grep Make;
CMakeLists.txt
Makefile
dragonshell > sleep 5 &; ls
PID 18480 is running in the background
builtin.cc  builtin.h  builtin.o  cmake-build-debug  CMakeLists.txt  dragonshell  dragonshell.cc  dragonshell.o  Makefile  readme.md  test.txt
dragonshell > ^C
dragonshell > exit
```
 

 
# Du-Shell-Utility
The du utility displays the sizes of the subdirectories of the tree rooted at the directory specified by its command-line argument. 

Develop a program named “ buNeDu ” that uses a post-order traversal to display the sizes of the subdirectories in a tree rooted at the specified starting path.

- (i) A function named postOrderApply that has the following prototype
* int postOrderApply (char *path, int pathfun (char *path1)); *
which traverses the tree, starting at the path. It applies the pathfun function to each file that it encounters in the traversal. 
The function returns the sum of the positive return values of pathfun, or -1 (if it failed to traverse any subdirectory)
- (ii) A function named sizepathfun (possibly to use for pathfun function defined in section (i) ) that has the following prototype
* int sizepathfun (char *path); *
The function outputs path along with other information obtained by calling stat or lstat for path .
The function returns the size in blocks of the file given by path or -1 if path does not corresponds to an ordinary file. 


 The program buNeDu when called with the argument rootpath as * buNeDu [-z] rootpath * with the function calls sizepathfun and postOrderApply should output the size of each directory followed by its pathname. When used with no extra arguments the size of the directory does not count the size of the subtrees of that directory. 
 However when additional argument ‘-z ‘ is introduced the size of the directory contains the sizes of all subtrees that the directory contains (note the difference). 
 If the pathname is a specific file, print an informative message but no size. 

 When none or meaningless command line arguments are given to the program(s) the output should warn the user and print a usage, informing the user how the program should actually be called.

# Du-Shell-Utility with fork
Creating a new process for each directory to get the sizes.

**Each created processes will be responsible for:**
- Finding the size of the directory given by parent process (adding the sizes of subdirectories if -z option is given)(using pipe is forbidden),
- Creating new processes to find the sizes of subdirectories,
- Writing the PID of the process, size(in kilobytes(preferred) or bytes) and path of the directory to a single global file “<#studentid>sizes.txt”(example: 111044002sizes.txt). use a file lock as multiple processes shouldn’t write to the same file at the same time. 

After the child processes write all the size of all directories, main process will read the file, find the total sizes if needed and output to standard output. The order of output is important and it should be postorder. Finally, it will output the number of created child processes and exit.
Do not add the sizes of files or the directories pointed by a symbolic link. Just say that there is a special file. Do not show size of any file explicitly, just directories.
The program buNeDuFork with the argument rootpath will be called like below. The rootpath can be any path on the system, not just a path in the current directory.
./buNeDuFork [-z] rootpath

While executing, do not communicate between any process to find total size for -z option, just write your local result to “<#studentid>sizes.txt” file. When all child processes finish, parse that file calculate to total sizes.


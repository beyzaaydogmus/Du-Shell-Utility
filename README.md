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
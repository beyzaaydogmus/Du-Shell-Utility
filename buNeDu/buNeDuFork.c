#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>

int depthFirstApply (char *path, int pathfun(char *path1),int fdir);
int sizepathfun (char *path); //The function returns the size in blocks of the file given by path
int ParsingCalculate(int fdir);
int flag = 0; //if there is -z argument flag will be 1 


struct directory{//its for parsingcalculate
   int size;  //size of directories 
   char path[1024]; //path of directories
   int pID; //process id number of directories
};

int main(int argc, char *argv[]){
	
	int file_descriptor;
	file_descriptor = open("151044034sizes.txt",O_RDWR | O_CREAT | O_TRUNC,  S_IREAD | S_IWRITE | S_IRWXG | S_IROTH);
	char * rootpath;
	if(argc==1){  
		fprintf(stderr,"there is no path, Usage : %s [path] \n", argv[0]);
		return 0;
	}
	
	else if(argc==2 && strcmp(argv[1],"-z") == 0 ){
		fprintf(stderr,"there is no path, Usage : %s [%s] [path] \n", argv[0],argv[1]); 
		return 0;
		
	}
	else if(argc==2 && strcmp(argv[1],"-z") != 0 )	rootpath = argv[1];
	else if(argc==3 && strcmp(argv[1],"-z") == 0 ){
		rootpath = argv[2];
		flag = 1;
	}

    else{ 

    	fprintf(stderr, "Invalid arguments, Usage : %s [-z] [path] \nor Usage : %s [path] \n", argv[0],argv[0]);
    	return 0;
    }

    switch (fork()){
        case 0:

            depthFirstApply(rootpath,sizepathfun,file_descriptor);
            break;

        default:
			printf("PID\tSIZE\tPATH\n");
			printf("\n");
            wait(0); //for child
            printf("%d child processes created. Main process is %d.\n", ParsingCalculate(file_descriptor),getpid());
			close(file_descriptor);

            break;
    }

    return 0;
}

int depthFirstApply (char *path, int pathfun(char *path1),int fdir){

    int total = 0;
    struct dirent *de;
    DIR * dr = opendir(path);
	if(dr =='\0'){
		fprintf(stderr,"Could not open directory, Usage : \"%s\"\n",path);
		return -1;
	}
	struct flock fl;

	while ((de=readdir(dr)) != '\0'){

		char* paths = (char*)malloc(1024); //to hold all paths allocate memory
		paths[0] = '\0';
		//concatanate
		paths = strcat(paths, path);
		paths = strcat(paths, "/");
		paths = strcat(paths, de->d_name);

		if(de -> d_name[0] != '.' || (de->d_name[1] != '.' && de->d_name[1] != 0)){
			
	        if(de -> d_type == DT_REG){
	            total = total + pathfun(paths);//do pathfun function if this is a regular file
				fl.l_type = F_WRLCK; // read/write lock
				fcntl(fdir, F_SETLKW, &fl); //set lock
	        }
	        else if(de -> d_type == DT_DIR){
	        	if(fork()==0){ //child created
					depthFirstApply(paths,sizepathfun,fdir); 
					closedir(dr);
					free(paths);
					return 0;
				}
			}

	        else{ // if this is not both directory and regular file thats mean its a special file
				printf("%d\t - \tSpecial file: %s\n",getpid(), de -> d_name);
			}
	    }
	    free(paths); //free allocated memory for paths
	}

	

	dprintf(fdir, "%s %d %d\n", path, total, getpid());
	fl.l_type = F_UNLCK; // read/write unlock
	fcntl(fdir, F_SETLKW, &fl); //unset lock
	close(fdir); 

	while (0 <= wait(0)); //wait for all child
	closedir(dr);
    return total;
}

int helper (char* A, char* B){
	for(int k=0;k<strlen(B);k++){
		if(!(B[k] == A[k])) return 0;
	}
	return 1;
}

int ParsingCalculate(int fdir){
	int count = 0;
	struct directory dirc[sizeof(struct directory)]; 
	lseek(fdir,0,SEEK_SET); // Offset is to be measured in absolute terms.
	FILE* file = fdopen(fdir,"r"); //open for reading

	char *readed = '\0';
    size_t length = 0;
	
	int w =0;
	while (!(getline(&readed, &length, file) == -1)) {
		sscanf(readed,"%s %d %d",dirc[w].path,&dirc[w].size,&dirc[w].pID);
		count++; //number of child
		w++;
	}

    if(flag == 1){
    	for(int i=0;i<count;++i){
			for(int j=i+1;j<count;++j){
				if(helper(dirc[j].path,dirc[i].path))
					dirc[i].size += dirc[j].size;
				
			}
		}
	}

	//post order
	for(int i=count; i>0; i--){
		printf("%d\t%d\t%s\n",dirc[i-1].pID, dirc[i-1].size, dirc[i-1].path);
	}
	free(readed);
	fclose(file);

	return count;
}


int sizepathfun (char *path){

    struct stat fileStat;
    int st = stat(path,&fileStat);
    int size=-1;
    if(st!=-1)
        size=(fileStat.st_size/1024);//converting bytes to kilobytes

    return size;
}


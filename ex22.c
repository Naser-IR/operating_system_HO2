#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <limits.h>
#include <strings.h>
#include <libgen.h>

int listdir(const char *path,char* comp,char l2[150],char l3[150],int fd2,int fd3);
int handle( char *path,char* comp,char l2[150],char l3[150],int fd2,int fd3);
int gcc(char* n,char* p,char* comp,char l2[150],char l3[150],int fd2,int fd3);
int result(char*n,int s,int fd2);


char* merge(char* str1, char* str2) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    char* str = (char*) malloc(len1 + len2 +1);
    int i = 0;
    for (; i < len1; ++i) {
        str[i] = *str1 ++;
    }
    for (; i < len2 + len1; ++i) {
        str[i] = *str2 ++;
    }
    str[i] = '\0';
    return str;
}

int main(int argc, char**argv) {
    if (argc !=2 ) {
        printf("not enough argument\n");
        return 0;
    }
    char l1[150];
    char l2[150];
    char l3[150];
    int fd2;
    int fd3;
    char *path;
    int f1;
    path = argv[1];
    f1 = open(path,O_RDONLY); //close at the end
    if(f1 == -1) {
        return -1;
    }
    // save every line in different variable
    //printf("%s",l1);
    dup2(f1,0);
    scanf(" %[^\n]s", l1);
    scanf(" %[^\n]s", l2);
    scanf(" %[^\n]s", l3);
    close(f1);
    char pwd[150];
    getcwd(pwd,150);
   // path =dirname(argv[1]);
    char * x1 = merge(merge(pwd,"/"),"errors.txt");
    char * x3 = merge(merge(pwd,"/"),"results.csv");

    char* comp = merge(merge(pwd,"/"),"comp.out");

    char* input = merge(merge(pwd,"/"),l2);
    if(access(input,F_OK)==-1){
        input = l2;
    }
    
    char* output = merge(merge(pwd,"/"),l3);
    if(access(output,F_OK)==-1){
        output = l3;
    }
    if(access(l1,F_OK)==-1){
        return -1;
    }
    if(access(input,F_OK)==-1){
        return -1;
    }
    if(access(output,F_OK)==-1){
        return -1;
    }
    fd2 = open(x3, O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    //char *tmname = "/Users/naser-ir/Desktop/files/errors.txt";
    fd3 = open(x1, O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    // at the end we save the first path then were gona fix that
 //   chdir(path);
   if(dup2(fd3, 2) == -1)
       perror("Error in : dup2");
    //listdir(l1,l1,l2,l3,fd2,fd3);
    listdir(l1,comp,input,output,fd2,fd3);
    close(fd2);
    close(fd3);
   // listdir("students");

   // start to check all the folders in the folder we got at the first line

}
int listdir(const char *path,char* comp,char l2[150],char l3[150],int fd2,int fd3){
    struct dirent *entery ;
    DIR * dp;
    dp = opendir(path);
    if(dp == NULL){
        perror("Error in : opendir");
        return -1;
    }
    if(chdir(path) == -1) {
                perror("Error in : chdir");
                return -1;
            }
    while ((entery = readdir(dp))) {
        if(entery->d_type == DT_DIR &&(entery->d_name[0] != '.')) {

         //  char t[100];

            handle(entery->d_name,comp,l2,l3,fd2,fd3);
            chdir("..");
        }
    }
    closedir(dp);
    return 0;
}
int handle( char *path,char* comp,char l2[150],char l3[150],int fd2,int fd3) {
    int flag=0;
    struct dirent *entery ;
    DIR * dp;
    dp = opendir(path);
    if(dp == NULL){
         perror("Error in : opendir");
        return -1;
    }
    chdir(path);
    while ((entery = readdir(dp))) {
        char * end = strrchr(entery->d_name, '.');
        if(end !=NULL) {
            if ((strcmp(end, ".c") == 0) && (entery->d_type != DT_DIR)) {
                gcc(entery->d_name, path,comp,l2,l3,fd2,fd3);
                flag++;
            }
        }
    }
    if (flag ==0){
        result(path,-1,fd2);
    }
    closedir(dp);
    return 0;
}
int gcc(char* n,char * p,char* comp,char l2[150],char l3[150],int fd2,int fd3) {
    int stat;
    pid_t pid;
    pid = fork();

    if (pid == 0) {
       // chdir(p);
        char *z[3];
        int l;
        z[0] = "gcc";
        z[1] = n;
        z[2] = NULL;
        l = execvp(z[0], z);
        if (l == -1) {
            perror("execvp failed");
            printf("\n");
            exit(0);
        }
        // first we need another fork to run the a.out with the input then write it to a output file
        // at the end we compare the output file with the one that we have
    } else {
        wait(&stat);
        if (pid == -1){
            perror("Error in : fork");
            return -1;
        }
        if (stat > 0) {
            result(p,-2,fd2);
        } else {
            //chdir(p);
            pid_t pid1;
            int fd;
            int fd1;
            fd1 = open(l2, O_RDONLY);
            char *tmpname = "./output.txt";
            fd = open(tmpname, O_CREAT | O_TRUNC | O_WRONLY,
                      S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
            pid1 = fork();
            if (pid1 == 0) {
                char *r[2];
                int l;
                r[0] = "./a.out";
                //r[1] = l2;
                r[1] = NULL;
                //we need to open a new file then print the output in it;
                //int fd;

               // fd1 = open(l2, O_RDONLY);
                dup2(fd, 1);
                dup2(fd1, 0);
                l = execvp(r[0], r);
                if (l == -1) {
                    perror("execvp failed");
                    printf("\n");
                    exit(0);
                }
//                close(fd);
//                close(fd1);

                // first we need another fork to run the a.out with the input then write it to a output file
                // at the end we compare the output file with the one that we have
            } else {
                wait(&stat);
                if (pid1 == -1){
                  perror("Error in : fork");
                  return -1;
                  }
                close(fd);
                close(fd1);
                pid_t pid2;
                pid2 = fork();
               // printf("%s\n",l3);
                int childExitStatus;
                if (pid2 == 0) {
                    char *r[4];
                    int l;
                    r[0] = comp;
                    r[1] = l3;
                    r[2] = "./output.txt";
                    r[3] = NULL;
                    //we need to open a new file then print the output in it;
                    l = execvp(r[0], r);
                    if (l == -1) {
                        perror("execvp failed");
                        printf("\n");
                        exit(0);
                    }
                } else {
                    waitpid(pid2, &childExitStatus, 0);
                    if (pid2 == -1){
                      perror("Error in : fork");
                      return -1;
                  }
                    int s = WEXITSTATUS(childExitStatus);
                   // printf("%d",s);
                    result(p,s,fd2);
                    remove("output.txt");
                    remove("a.out");
                }
                // first we need another fork to run the a.out with the input then write it to a output file
                // at the end we compare the output file with the one that we have
            }
        }
    }

}
int result(char *name,int s,int fd2){
    write(fd2,name, strlen(name));
    write(fd2,",", 1);
    if(s==1) {
        write(fd2, "100", 3);
        write(fd2,",", 1);
        write(fd2, "EXCELLENT", 9);
        write(fd2, "\n", 2);
    }else if(s==3){
        write(fd2, "75", 2);
        write(fd2,",", 1);
        write(fd2, "SIMILAR", 7);
        write(fd2, "\n", 2);
    }else if(s==-1){
        write(fd2, "0", 1);
        write(fd2,",", 1);
        write(fd2, "NO_C_FILE", 9);
        write(fd2, "\n", 2);
    }else if(s==-2){
        write(fd2, "10", 2);
        write(fd2,",", 1);
        write(fd2, "COMPILATION_ERROR", 17);
        write(fd2, "\n", 2);
    }else{
        write(fd2, "50", 2);
        write(fd2,",", 1);
        write(fd2, "WRONG", 5);
        write(fd2, "\n", 2);
    }
}

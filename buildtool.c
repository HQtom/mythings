#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>  
int counter = 0;
int subd = 0;
char gcc1[10] = "gcc";
char gcc2[10] = "-c";
char bash = '/'; // a char buffer for comparasion

char startPath[260];  // string to store the start dir path

// the function which handling the files.
void* fHandler(void* arg) {
    
    char* cm = malloc(260);
    cm = (char*)arg;
    char cm2[260];
    //printf("%s\n",cm);
    sprintf(cm2, "%s %s %s", gcc1, cm, gcc2);  // print the command into a string and call by system();

    if (system(cm2) != 0) {
        perror("compile failed");  //when gcc compiling failed, exit and print error 
        exit(1);
   }

   

}


// function of go thru all the files and dir by recursion
int Recur(const char* strpathname,char* name)
{
    char files[1250];
    
    strcat(files, gcc1);
    system("pwd");   // show current place
    DIR* dir;   
    
   

    if ((dir = opendir(strpathname)) == 0) return -1; 

    struct dirent* stdinfo;

    while (1)
    {
        if ((stdinfo = readdir(dir)) == 0) break;   

        if (strncmp(stdinfo->d_name, ".", 1) == 0) continue;  

        if (stdinfo->d_type == 4)  // if is a dir
        {
            char* dname = malloc(sizeof(stdinfo->d_name));
            dname = stdinfo->d_name;
            char strchdpath[260];
            sprintf(strchdpath, "%s/%s", strpathname, stdinfo->d_name);
            Recur(strchdpath,dname);


        }else if (stdinfo->d_type == 8) {
            //sprintf(files, " %s %s", files, stdinfo->d_name);
            strcat(files, " ");
            strcat(files,stdinfo->d_name);
         
           while (1) {
               if (counter < 4) {
                   break;
               }
                sleep(1);  //sleep 1 second to waitif the counter is less than 4
            }
            pthread_t th;    // creating thread
            counter++;
            printf("\n");
            chdir(strpathname);
            char fname[260];
            sprintf(fname, "%s", stdinfo->d_name);
            printf("compile %s\n", stdinfo->d_name);
            pthread_create(&th, NULL,fHandler, fname);
            pthread_join(th, NULL);
            counter--;
        }
       
        
    
        
    }
    char arcmd[1000];
    strcat(arcmd, "ar crv");
    sprintf(arcmd, "ar crv %s.a *.o", name);
    system(arcmd);
    char mvcmd[300];
    sprintf(mvcmd,"mv %s.a %s",name,startPath);  // create the command that move the files to the top dir
    system(mvcmd); 

    closedir(dir);  
    
}








int main() {
    char strpwd[301];
    char buff[20];
    memset(strpwd, 0, sizeof(strpwd));
    getcwd(strpwd, 300);
    strcpy(startPath, strpwd);
    int c = -1;
    for (int i = strlen(strpwd); i > 0; i--) {      // two loops to get the name of the top directory
        if (strpwd[i] == bash) {
            break;
        }
        buff[c] = strpwd[i];
        c++;
    }
    
    c = 0;
    char* main;
    main = malloc(sizeof(buff));
    for (int j = strlen(buff)-1; j >= 0; j--) {   // now get the correct name of dir by reading the path
        main[c] = buff[j];
        c++;
        
    }
    Recur(strpwd,main);   
    char cmd[50] = "gcc *.a -o ";   // compile all .a files
    strcat(cmd, main);
    system(cmd);
}
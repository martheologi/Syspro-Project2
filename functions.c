#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

#include "structs.h"
#include "functions.h"

int numLength(int num){
    if (num < 10)
        return 1;
    return 1 + numLength(num / 10);
}

void DirectoriesCheck(char* commondir, char* inputdir, char* mirrordir, char* logfile){
    DIR* common = opendir(commondir);
    if(common == NULL){
        mkdir("./common", 0777);
    }
    else{
        closedir(common);
    }

    DIR* input_dir = opendir(inputdir);
    if(input_dir){
        closedir(input_dir);
    }
    else{     //an den uparxei to id_input kanw exit
        perror("opendir/input_dir");
        exit(1);
    }

    DIR* mirror_dir = opendir(mirrordir);
    if(mirror_dir){ //an uparxei to id_mirror kanw exit
        closedir(mirror_dir);
        perror("opendir/mirror_dir");
        exit(1);
    }
    else{           //alliws to dhmiourgw
        char* mydir = malloc(strlen(mirrordir)+1);
        strcpy(mydir, mirrordir);
        mkdir(mydir, 0777);
        free(mydir);
    }

    FILE* log_file = fopen(logfile, "rb+");
    if(log_file == NULL){
        log_file = fopen(logfile, "wb");
    }
    fclose(log_file);

    return;
}

char* readfifo(int fd, char* buffer, int buff_size){
    //char* buffer = malloc(buff_size);
    while(1){
        if(read(fd, buffer, buff_size) > 0){
            return buffer;
        }
    }
}

void removeDir(char* path){
    struct dirent* ent;
    DIR* mirror = opendir(path);

    while(ent=readdir(mirror)){
        if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
            continue;

        struct dirent* INent;
        char* newpath = malloc(strlen(path)+strlen(ent->d_name)+2);
        sprintf(newpath, "%s/%s", path, ent->d_name);

        DIR* directory = opendir(newpath);
        if(!directory) perror("poutses");

        while(INent=readdir(directory)){
            if(strcmp(INent->d_name,".")==0 || strcmp(INent->d_name,"..")==0)
                continue;
            char* filepath = malloc(strlen(newpath)+strlen(INent->d_name)+2);
            sprintf(filepath, "%s/%s", newpath, INent->d_name);
            remove(filepath);
            free(filepath);
        }
        closedir(directory);
        rmdir(newpath);
        free(newpath);
    }
    closedir(mirror);
    rmdir(path);

    return;
}

void removeID(char* path, int id){
    struct dirent* ent;
    DIR* common = opendir(path);
    int dirID;

    while(ent=readdir(common)){
        if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0)
            continue;

        sscanf(ent->d_name, "%d.id", &dirID);

        if(dirID == id){
            char* filepath = malloc(strlen(path)+strlen(ent->d_name)+2);
            sprintf(filepath, "%s/%s", path, ent->d_name);
            remove(filepath);
            free(filepath);
        }
    }
    closedir(common);
    return;
}

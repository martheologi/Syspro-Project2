#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

#include "structs.h"
#include "functions.h"

static int flag = 0;

void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    printf("\n INTERAAAPT \n");
    flag = 1;
    //fflush(stdout);
}

int main(int argc,char* argv[]){

    int id = atoi(argv[1]);
    int buff_size = atoi(argv[5]);
    pid_t reader;
    pid_t writer;

    //elegxoi gia ta directories kai to logfile
    DirectoriesCheck(argv[2], argv[3], argv[4], argv[6]);

//dhmiourgw to arxeio .id sto common
    FILE* fp;
    char* myPid;
    char* fileName = malloc(strlen(argv[1]) + 1 + strlen("./common/.id"));
    sprintf(fileName, "./common/%d.id", id);
    fp = fopen(fileName, "rb+");

    if(fp == NULL){
        fp = fopen(fileName, "wb");
        int length = numLength(getpid());
        myPid = malloc(length+1);
        sprintf(myPid, "%d", getpid());
        fputs(myPid, fp);

        free(myPid);
        fclose(fp);
    }
    else{      //an uparxei hdh kanw exit
        printf("%s exixts\n", fileName);
        fclose(fp);
        free(fileName);
        exit(1);
    }
    free(fileName);


//elegxw tous clients sto common
    IDsListNode* IDsList = initializeIDsList(id);
    struct dirent* ent;
    int newid;
    int fd;

    DIR* common = opendir(argv[2]);
    if(!common){
        perror("opendir/common");
        exit(1);
    }
    else{
        while(1){
            int common_counter=0;
            while((ent = readdir(common)) != NULL){
                int t1, t2;
                if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0 || sscanf(ent->d_name, "%d_to_%d.fifo", &t1, &t2)==2)
                    continue;

                sscanf(ent->d_name, "%d.id", &newid);
                common_counter++;

                if(IDsListAppend(IDsList, newid)!=0){
                    char* RpipeName = malloc(strlen("common/_to_.fifo")+numLength(id)+numLength(newid)+1);
                    char* WpipeName = malloc(strlen("common/_to_.fifo")+numLength(id)+numLength(newid)+1);

                    if((writer = fork()) < 0){
                        perror("fork/writer");
                    }

                    if(writer!=0){
                        if((reader = fork()) < 0){
                            perror("fork/reader");
                        }
                    }

                    sprintf(WpipeName, "common/%d_to_%d.fifo", id, newid);//writer's fifo
                    if(mkfifo(WpipeName,0666)==0){
                        if((fd = open(WpipeName, O_WRONLY)) == -1){
                            perror("open/write_fd");
                        }
                    }
                    sprintf(RpipeName, "common/%d_to_%d.fifo", newid, id);//reader's fifo
                    if(mkfifo(RpipeName,0666)==0){
                        if((fd = open(RpipeName, O_RDONLY|O_NONBLOCK)) == -1){
                            perror("open/read_fd");
                        }
                    }
//WRITEEEEEEER
                    if(writer == 0){
                        struct dirent* inpt_ent;
                        DIR* input_dir = opendir(argv[3]);
                        rewinddir(input_dir);
                        int inpt_file;
                        char* name_size = malloc(buff_size);
                        char* file_size = malloc(buff_size);
                        char* buffer = malloc(buff_size);

                        if((fd = open(WpipeName, O_WRONLY)) == -1){
                            perror("open/write_fd");
                        }

                        //id1_to_id2
                        while((inpt_ent = readdir(input_dir)) != NULL){
                            char* file_name = malloc(strlen("_input/")+numLength(id)+strlen(inpt_ent->d_name)+1);

                            if(strcmp(inpt_ent->d_name,".")==0 || strcmp(inpt_ent->d_name,"..")==0){
                                free(file_name);
                                continue;
                            }

                            sprintf(file_name, "%d_input/%s", id, inpt_ent->d_name);

                            inpt_file = open(file_name, O_RDONLY);

                            if(inpt_file == -1){
                                printf("%s ", file_name);
                                perror("open/input_file");
                            }

                            sprintf(name_size, "%zu", strlen(inpt_ent->d_name));

                            //grafw to mege8os tou onomatos tou arxeiou
                            strncpy(buffer, name_size, buff_size);
                            if(write(fd, buffer, buff_size) != buff_size){
                                perror("write/name_size");
                            }
                            memset(buffer, 0, buff_size);

                            //grafw to onoma tou arxeiou
                            strncpy(buffer, inpt_ent->d_name, buff_size);
                            if(write(fd, buffer, buff_size) != buff_size){
                                perror("write/file_name");
                            }
                            memset(buffer, 0, buff_size);

                            //grafw to size tou arxeiou
                            struct stat st;
                            stat(file_name, &st);
                            int size = st.st_size;

                            sprintf(file_size, "%d", size);
                            strncpy(buffer, file_size, buff_size);
                            if(write(fd, buffer, buff_size) != buff_size){
                                perror("write/file_size");
                            }
                            memset(buffer, 0, buff_size);

                            //grafw to arxeio
                            read(inpt_file, buffer, buff_size);
                            if(write(fd, buffer, buff_size) != buff_size){
                                perror("write/file");
                            }
                            memset(buffer, 0, buff_size);
                            close(inpt_file);

                            free(file_name);
                        }

                        strncpy(buffer, "00", buff_size);
                        if(write(fd, buffer, buff_size) != buff_size){
                            perror("write/00");
                        }

                        free(WpipeName);
                        free(name_size);
                        free(file_size);
                        free(buffer);
                        close(fd);
                        closedir(input_dir);
                        exit(57);
                    }
//READEEEEER
                    else if(reader == 0){
                        char* read_buff = malloc(buff_size);

                        if((fd = open(RpipeName, O_RDONLY|O_NONBLOCK)) == -1){
                            perror("open/read_fd");
                        }

                        while(1){
                            //diavazw to mhkos tou onomatos
                            read_buff = readfifo(fd, read_buff, buff_size);
                            int nameS = atoi(read_buff);
                            if((read_buff[0]=='0') && (read_buff[1]=='0')){
                                break;
                            }
                            memset(read_buff, 0, buff_size);

                            //diavazw to onoma tou arxeiou
                            read_buff = readfifo(fd, read_buff, buff_size);
                            char* fname = malloc(strlen(read_buff)+1);
                            strcpy(fname, read_buff);
                            memset(read_buff, 0, buff_size);

                            //diavazw to mege8os tou arxeiou
                            read_buff = readfifo(fd, read_buff, buff_size);
                            int fileS = atoi(read_buff);
                            memset(read_buff, 0, buff_size);

                            //diavazw to periexwmeno
                            read_buff = readfifo(fd, read_buff, buff_size);
                            char* periex = malloc(buff_size);
                            strncpy(periex, read_buff, buff_size);
                            memset(read_buff, 0, buff_size);

                            //grafw sto log_file
                            FILE* lf = fopen(argv[6], "a");
                            fprintf(lf, "file name: \"%s\"\tbytes: %d\n", fname, nameS+fileS);
                            fclose(lf);

                            //pernaw to arxeio sto mirror
                            char* directory = malloc(strlen("_mirror/")+numLength(id)+numLength(newid)+1);
                            sprintf(directory, "%d_mirror/%d", id, newid);

                            char* mirror_file = malloc(strlen("_mirror//")+strlen(fname)+numLength(id)+numLength(newid)+2);
                            sprintf(mirror_file, "%d_mirror/%d/%s", id, newid, fname);

                            DIR* mirror_dir = opendir(directory);
                            if(!mirror_dir){
                                mkdir(directory, 0777);
                            }
                            FILE* mf = fopen(mirror_file, "w+");
                            fprintf(mf, "%s\n", periex);

                            fclose(mf);
                            free(mirror_file);
                            free(directory);
                            free(fname);
                            free(periex);

                        }
                        free(RpipeName);
                        free(read_buff);
                        close(fd);
                        exit(57);
                    }
//FATHEEEEEER
                    else{
                        int status1, status2;
                        waitpid(writer,&status1,0);
                        while(waitpid(reader,&status2,WNOHANG)==0){
                            sleep(1);
                        }
                        if(WIFEXITED(status2)){
                            printf("transaction with %d is complete\n", newid);
                        }
                        //perimenei signal apo ton xrhsth (ctrl+C, ctrl+\)
                        static struct sigaction act;
                        act.sa_handler = sigintHandler;
                        sigfillset(&(act.sa_mask));
                        sigaction(SIGINT, &act, &act);
                        //perimenei signal apo ton xrhsth (ctrl+C, ctrl+\)
                        if(flag==0) //evala global flag pou to eida stis diafaneies
                            continue;
                        if(flag==1){
                            removeDir(argv[4]);
                            removeID(argv[2], id);
                            free(WpipeName);
                            free(RpipeName);
                            FreeIDsList(IDsList);
                            closedir(common);
                            exit(57);
                        }
                    }
                }
            }
            //elegxw an exei vgei kapoios client
            if(common_counter < numOfNodes(IDsList)){
                IDsListNode* tmp = IDsList;
                //psaxnw gia ka8e stoixeio ths lista tou client an uparxei kai sto common
                while(tmp!=NULL){
                    int found = 0;
                    rewinddir(common);
                    while((ent = readdir(common)) != NULL){
                        int t1, t2, tmpid;
                        if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0 || sscanf(ent->d_name, "%d_to_%d.fifo", &t1, &t2)==2)
                            continue;
                        sscanf(ent->d_name, "%d.id", &tmpid);
                        if( tmp->id == tmpid){
                            found = 1;
                            break;
                        }
                    }
                    //an den uparxei kanw break
                    if(found == 0){
                        break;
                    }
                    tmp = tmp->next;
                }
                //afairw ton client pou efuge apo th lista mou
                int removed_id = tmp->id;
                if(IDsListRemove(IDsList, tmp->id) != NULL){
                    printf("removed client %d\n", removed_id);
                    pid_t remover = fork();
//REMOOOOOVEEEER
                    if(remover==0){
                        char* iddir = malloc(strlen("_mirror/")+numLength(removed_id)+numLength(id)+1);
                        sprintf(iddir, "%d_mirror/%d", id, removed_id);
                        DIR* mirrorid = opendir(iddir);
                        int clid;
                        struct dirent* ment;
                        while(ment=readdir(mirrorid)){
                            if(strcmp(ment->d_name,".")==0 || strcmp(ment->d_name,"..")==0)
                                continue;

                            sscanf(ment->d_name, "%d", &clid);

                            char* filepath = malloc(strlen(iddir)+strlen(ment->d_name)+2);
                            sprintf(filepath, "%s/%s", iddir, ment->d_name);
                            remove(filepath);
                            free(filepath);
                        }

                        closedir(mirrorid);
                        rmdir(iddir);
                        free(iddir);
                        exit(57);
                    }
                }
            }
            //elegxw periodika ka8e 2" to common
            closedir(common);
            sleep(1);
            common = opendir(argv[2]);
        }
    }

    return 0;
}

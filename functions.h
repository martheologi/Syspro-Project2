
////LIST
IDsListNode* initializeIDsList(int id);

int IDsListAppend(IDsListNode* ptr, int id);

IDsListNode* IDsListRemove(IDsListNode* curr, int id);
//int IDsListRemove(IDsListNode** head, int id);

int numOfNodes(IDsListNode* ptr);

void FreeIDsList(IDsListNode* ptr);

////FUNCTIONS
void DirectoriesCheck(char* commondir, char* inputdir, char* mirrordir, char* logfile);

int numLength(int n);

char* readfifo(int fd, char* buffer, int buff_size);

void sigintHandler(int sig_num);

void removeDir(char* path);

void removeID(char* path, int id);

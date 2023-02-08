
void UDP_init(void);
void UDP_cleanup(void);

void * UDP_getCommand(void*);
char* UDP_noCommand(void);
char* UDP_help(void);
char* UDP_getCount(void);
char* UDP_getLastN(int n);
char* UDP_getLength(void);
char* UDP_getHistory(void);
char* UDP_getDips(void);
void UDP_stop(void);
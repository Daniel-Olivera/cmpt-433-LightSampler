
//Init and cleanup of sockets
void UDP_init(void);
void UDP_cleanup(void);

//Retrieves a command from netcat through UDP connection
void * UDP_getCommand(void*);

//Performs the previous command
char* UDP_noCommand(void);

//Displays the help information
char* UDP_help(void);

//Displays the total number of light samples taken
char* UDP_getCount(void);

//Displays the last N samples taken
char* UDP_getLastN(int n);

//Displays the size and length of the history
char* UDP_getLength(void);

//Displays the entire history
char* UDP_getHistory(void);

//Displays the number of dips
char* UDP_getDips(void);

//Signals all threads to end their loops and join back to main
void UDP_stop(void);
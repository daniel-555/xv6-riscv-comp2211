#include "kernel/types.h"
#include "user/user.h"

int main() {
    char byte = 'P';
    char buffer;

    int PtoC[2];
    int CtoP[2];

    pipe(PtoC);
    pipe(CtoP);

    int pid;
    

    if (fork() == 0) {
        // Child Code
        pid = getpid();

        // close unused pipe ends
        close(PtoC[1]);
        close(CtoP[0]);

        // Read from pipe
        read(PtoC[0], &buffer, 1);
        close(PtoC[0]);

        printf("%d: Recieved Ping, ", pid);
        write(1, &buffer, 1);
        printf("\n");
        

        // Send pong
        buffer += 2;
        write(CtoP[1], &buffer, 1);
        close(CtoP[1]);

        exit(0);

    } else {
        // Parent Code
        pid = getpid();

        // Close unused pipe ends
        close(PtoC[0]);
        close(CtoP[1]);

        // Send ping
        write(PtoC[1], &byte, 1);
        close(PtoC[1]);

        
        // Read from child
        read(CtoP[0], &buffer, 1);
        printf("%d: Recieved Pong, ", pid);
        write(1, &buffer, 1);
        printf("\n");

        close(CtoP[0]);

    }

    return 0;
}
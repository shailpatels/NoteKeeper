#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

#include "note.h"

void makeDir(char * PATH){
    struct stat s;
    if ( stat( PATH, &s) == -1 ){
        int stat = mkdir(PATH, S_IRWXU | S_IRWXG | 
                               S_IROTH | S_IXOTH);
    
        if ( stat == - 1 ){
            perror("Could not make dir at \"%s\" : ");
            exit(EXIT_FAILURE);
        }
    }
}

void initHome(char * NOTES_PTH, char * HOME){

    strcpy(NOTES_PTH, HOME);
    strcat(NOTES_PTH, TGTPATH);

    //check if a notes dir exists
    makeDir( NOTES_PTH );
}

date getDate(){
    time_t t = time(NULL);
    struct tm now = * localtime( &t );

    return (date) { now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
                    now.tm_hour, now.tm_min, now.tm_sec};
}

char * getDirFromDate( date * d ){
    // yyyy-mm-dd

    char * ret;
    if ( (ret = malloc( sizeof( char ) * 10 )) == NULL ){
        perror("Failed to malloc ");
        exit(EXIT_FAILURE);
    }

    sprintf( ret, "%d-%d-%d", d->year, d->month, d->day );
    return ret;
}

char * getFileFromDate( date * d) {
    // hh:mm:ss

    char * ret;
    if ( (ret = malloc( sizeof( char ) * 10 )) == NULL ){
        perror("Failed to malloc ");
        exit(EXIT_FAILURE);
    }
 
    sprintf( ret, "%d:%d:%d", d->hour, d->min, d->sec );
    return ret;
}

void launchVim(char * PATH){
    char * ARG[] = { "/usr/bin/vim/", PATH, NULL}; 

    pid_t pid = fork();
    if ( pid == 0 ){
        execv( "/usr/bin/vim", ARG );
        exit( EXIT_SUCCESS );
    }else if ( pid > 0 ){
        int status;
        waitpid(pid, &status, 0);
    
        printf("[Process %d] : Exited with status %d\n", pid, WEXITSTATUS( status ));
    }else{
        perror("Failed to fork! ");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char ** argv){
    char * HOME = getpwuid( getuid() )->pw_dir;    
    char NOTES_PTH[ MAXBUFF ];
    initHome( NOTES_PTH, HOME); 

    //get date info  
    time_t t = time(NULL);
    struct tm now = * localtime( &t );
    
    date d = getDate(); 
    char * fname = argc == 1 ? getFileFromDate( &d ) : argv[1];
    char * dname = getDirFromDate( &d );

    strcat( NOTES_PTH, dname );
    strcat( NOTES_PTH, "/" );
    makeDir( NOTES_PTH );

    strcat( NOTES_PTH, fname );
    launchVim( NOTES_PTH );

    printf("Wrote \"%s\" to \"%s\"\n", fname, dname);
    printf("%s\n", NOTES_PTH);
    if ( argc == 1 )
        free( fname );

    free( dname );
}

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define FALSE 0
#define TRUE 1
#define INPUT_STRING_SIZE 80

#include "io.h"
#include "parse.h"
#include "process.h"
#include "shell.h"


int cmd_quit(tok_t arg[]) {
  printf("Bye\n");
  exit(0);
  return 1;
}

int cmd_help(tok_t arg[]);

int cmd_cd(tok_t arg[]){
   if(arg[0] == NULL)
     return 0;
   else
     chdir(arg[0]);
 return 1;

 }


/* Command Lookup table */
typedef int cmd_fun_t (tok_t args[]); /* cmd functions take token array and return int */
typedef struct fun_desc {
  cmd_fun_t *fun;
  char *cmd;
  char *doc;
} fun_desc_t;

fun_desc_t cmd_table[] = {
  {cmd_help, "?", "show this help menu"},
  {cmd_quit, "quit", "quit the command shell"},
  {cmd_cd, "cd", "change the directory"},
};

int cmd_help(tok_t arg[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    printf("%s - %s\n",cmd_table[i].cmd, cmd_table[i].doc);
  }
  return 1;
}

int lookup(char cmd[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0)) return i;
  }
  return -1;
}

void init_shell()
{
  /* Check if we are running interactively */
  shell_terminal = STDIN_FILENO;

  /** Note that we cannot take control of the terminal if the shell
      is not interactive */
  shell_is_interactive = isatty(shell_terminal);

  if(shell_is_interactive){

    /* force into foreground */
    while(tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp()))
      kill( - shell_pgid, SIGTTIN);

    shell_pgid = getpid();
    /* Put shell in its own process group */
    if(setpgid(shell_pgid, shell_pgid) < 0){
      perror("Couldn't put the shell in its own process group");
      exit(1);
    }

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);
    tcgetattr(shell_terminal, &shell_tmodes);
  }
  /** YOUR CODE HERE */
}

/**
 * Add a process to our process list
 */
void add_process(process* p)
{
  /** YOUR CODE HERE */
}

/**
 * Creates a process given the inputString from stdin
 */
process* create_process(char* inputString)
{
  /** YOUR CODE HERE */
  return NULL;
}


int shell (int argc, char *argv[]) {
  char *s = malloc(INPUT_STRING_SIZE+1); /* here we create a string and we allocarte memory for it, the reason we add oone is bcos a string always has that one character at the end \0*/
  tok_t *t;			/* we create token "array"  */
  int lineNum = 0;   
  int fundex = -1;     /**/
  pid_t pid = getpid();		/* get current processes PID (process iD)*/
  pid_t ppid = getppid();	/* get parents PID (built in) PID (ppid, cpid)*/
  pid_t cpid, tcpid, cpgid;

  init_shell(); /* this funcion is defined somewhere and is not built*/
  char a[120];
  printf("%s running as PID %d under %d\n",argv[0],pid,ppid);

 
  fprintf(stdout, "%d %s: ", lineNum, getcwd(*a,120));
  while ((s = freadln(stdin))){
    t = getToks(s); /* break the line into tokens */
    fundex = lookup(t[0]); /* Is first token a shell literal */
    if(fundex >= 0) {cmd_table[fundex].fun(&t[1]);}
    else {
      pid_t pid = fork();
	if(pid<0){
      printf("Error creating the child\n");
	}else if(pid==0){
	printf("Zinhle\n");
	execvp(*t,t);
}else{
wait(NULL);
}

   lineNum++;
    fprintf(stdout, "%d %s: ", lineNum, getcwd(*a,120));
  }
}

  return 0;
}

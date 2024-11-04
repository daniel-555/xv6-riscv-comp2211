#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

/* Read a line of characters from stdin. */
int getcmd(char *buf, int nbuf)
{

  // ##### Place your code here
  fprintf(2, ">>> ");
  memset(buf, 0, nbuf); // Reset the buffer after each command
  read(0, buf, nbuf);
  if (buf[0] == 0)
    return -1;
  return 0;
}

/*
  A recursive function which parses the command
  at *buf and executes it.
*/
__attribute__((noreturn)) void run_command(char *buf, int nbuf, int *pcp)
{

  /* Useful data structures and flags. */
  char *arguments[10];
  int numargs = 0;
  /* Word start/end */
  int ws = 1; // 1: no current word, 0: in the process of reading a word
  int we = 0; // the first index of the current word, called back when the word ends

  int redirection_left = 0;
  int redirection_right = 0;
  // char *file_name_l = 0;
  // char *file_name_r = 0;

  // int p[2];
  int pipe_cmd = 0;

  int sequence_cmd = 0;

  int i = 0;
  /* Parse the command character by character. */
  for (; i < nbuf; i++)
  {

    /* Parse the current character and set-up various flags:
       sequence_cmd, redirection, pipe_cmd and similar. */

    /* ##### Place your code here. */
    switch (buf[i])
    {
    case ' ':
    case '\n':
      if (ws == 0)
      {
        buf[i] = '\0';
        arguments[numargs++] = &buf[we];
        ws = 1;
      }
      break;

    // Character
    default:
      if (ws == 1)
      {
        we = i;
        ws = 0;
      }
      break;
    }

    if (!(redirection_left || redirection_right))
    {
      /* No redirection, continue parsing command. */
      continue;
      // Place your code here.
    }
    else
    {
      /* Redirection command. Capture the file names. */

      // ##### Place your code here.
    }
  }

  arguments[numargs] = 0;

  /*
    Sequence command. Continue this command in a new process.
    Wait for it to complete and execute the command following ';'.
  */
  if (sequence_cmd)
  {
    sequence_cmd = 0;
    if (fork() != 0)
    {
      wait(0);
      // ##### Place your code here.
    }
  }

  /*
    If this is a redirection command,
    tie the specified files to std in/out.
  */
  if (redirection_left)
  {
    // ##### Place your code here.
  }
  if (redirection_right)
  {
    // ##### Place your code here.
  }

  /* Parsing done. Execute the command. */

  /*
    If this command is a CD command, write the arguments to the pcp pipe
    and exit with '2' to tell the parent process about this.
  */
  if (strcmp(arguments[0], "cd") == 0)
  {
    // ##### Place your code here.
  }
  else
  {
    /*
      Pipe command: fork twice. Execute the left hand side directly.
      Call run_command recursion for the right side of the pipe.
    */
    if (pipe_cmd)
    {
      // ##### Place your code here.
    }
    else
    {
      // ##### Place your code here.
      exec(arguments[0], arguments);
    }
  }

  exit(0);
}

int main(void)
{

  static char buf[100];

  int pcp[2];
  pipe(pcp);

  /* Read and run input commands. */
  while (getcmd(buf, sizeof(buf)) >= 0)
  {
    if (fork() == 0)
    {
      run_command(buf, 100, pcp);
    }
    else
    {
      wait(0);
    }
    /*
      Check if run_command found this is
      a CD command and run it if required.
    */
    // int child_status;
    // ##### Place your code here
  }
  exit(0);
}

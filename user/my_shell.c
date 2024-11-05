#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

/* Read a line of characters from stdin. */
int getcmd(char *buf, int nbuf)
{

  // ##### Place your code here
  fprintf(2, ">>> ");
  memset(buf, 0, nbuf); // Reset the buffer after each command
  read(0, buf, nbuf);
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
  char *file_name_l = 0;
  char *file_name_r = 0;

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
    case '\t':
      if (ws == 0)
      {
        buf[i] = '\0';
        arguments[numargs++] = &buf[we];
        ws = 1;
      }
      break;

    // Left Redirect
    case '<':
      redirection_left = 1;
      buf[i] = '\0'; // Terminate last argument

      while (buf[++i] == ' ' || buf[i] == '\t')
        ; // Skip over whitespace to find the start of the filename

      file_name_l = &buf[i];

      while (buf[i] != ' ' && buf[i] != '\n' && buf[i] != '\t' && buf[i] != '\0')
      {
        i++; // Find the end of the file name
      }
      buf[i] = '\0';
      break;

    // Right Redirect
    case '>':
      redirection_right = 1;
      buf[i] = '\0'; // Terminate last argument

      while (buf[++i] == ' ' || buf[i] == '\t')
        ; // Skip over whitespace to find the start of the filename

      file_name_r = &buf[i];

      while (buf[i] != ' ' && buf[i] != '\n' && buf[i] != '\t' && buf[i] != '\0')
      {
        i++; // Find the end of the file name
      }
      buf[i] = '\0';
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

    if (redirection_left || redirection_right)
    {
      break;
    }
  }

  arguments[numargs] = 0;

  int j = 0;
  while (arguments[j])
  {
    printf("%d: %s\n", j, arguments[j]);
    j++;
  }
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
    // Set file_name_l as stdin
    printf("file: %s\n", file_name_l);
    close(0);
    open(file_name_l, O_RDONLY);
  }
  if (redirection_right)
  {
    // ##### Place your code here.
    // Set file_name_r as stdout
    printf("file: %s\n", file_name_r);
    close(1);
    open(file_name_r, O_CREATE | O_WRONLY | O_TRUNC);
  }

  /* Parsing done. Execute the command. */

  /*
    If this command is a CD command, write the arguments to the pcp pipe
    and exit with '2' to tell the parent process about this.
  */
  if (strcmp(arguments[0], "cd") == 0)
  {
    // ##### Place your code here.
    write(pcp[1], arguments[1], strlen(arguments[1]));
    exit(2);
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
      if (exec(arguments[0], arguments) < 0)
      {
        fprintf(2, "exec %s failed\n", arguments[0]);
        exit(1);
      }
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
      /*
        Check if run_command found this is
        a CD command and run it if required.
      */
      int child_status;
      // ##### Place your code here

      wait(&child_status);
      if (child_status == 2)
      {
        char dir[DIRSIZ] = "";
        read(pcp[0], dir, DIRSIZ);
        if (chdir(dir) < 0)
        {
          fprintf(2, "cannot cd %s\n", dir);
        }
      }
    }
  }
  exit(0);
}

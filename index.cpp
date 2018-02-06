// Patrick Dayton
// CS 5348
// Prof. Ozbirn
// Due 24 February 2018


// TO RUN APPLICATION
// g++ index.cpp -o main
// ./main sample1.txt 30

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>
using namespace std;
// #include <wait.h>



//Load program from txt FILE
//base code from https://www.codingunit.com/c-tutorial-file-io-using-text-files
// void read_ints (const char* file_name)
int loadProgram()
{
  FILE *ptr_file;
	char buf[1000];
  char ins_str[50];
  int memory[2000];
  int line = 0;

  // zero out program portion of Array
  // as I was getting weird numbers starting at 436.
  int n = sizeof(memory)/sizeof(memory[0]);
  for (size_t i = 0; i < n; i++) {
    memory[i]=0;
  }

	ptr_file =fopen("sample3.txt","r");
	if (!ptr_file)
 		return 1;

	while (fgets(buf, sizeof(buf), ptr_file) != NULL)
    if (buf[0] != '\n')
      {
        int num;
        if(buf[0] == '.')
        {
          sscanf(buf, ".%d %*s\n", &num);
          line = num;
          memory[line] = num;
        }
        else
        {
          line++;
          sscanf(buf, "%d %*s\n", &num);
          memory[line] = num;
        }
      }
	fclose(ptr_file);


  // for(int j = 1; j < 2000; j++) {
  //     printf("%d", j);
  //     printf("%s", "\t");
  //     printf("%d", memory[j]);
  //     printf("%s", "\n");
  // }

 	return 0;
}

int main(int argc, char** argv)
{
   //TODO ignore error checks for simplicity


   //Create Registers
   int PC, SP, IR, AC, X, Y;

   //Create Memory Array
   int mem[2000]; //2000 integer entries, 0-999 for the user program, 1000-1999 for system code

   //Load txt file into memory
   loadProgram();

   string file_name = argv[1];
   int timer = atoi(argv[2]);
   cout<<file_name<<'\n';
   cout<<timer<<'\n';


   // int x, y, z;
   // int pd1[2], pd2[2];
   //
   // x = pipe(pd1);
   // y = pipe(pd2);
   //
   // z = fork();
   //
   // if (z == 0)
   // { // child
   //    int a=5;
   //    int b=10;
   //    char req;
   //
   //    read(pd1[0], &req, sizeof(req));
   //    while (req != 'X')
   //    {
   //  	  if (req == 'A')
   //  	     write(pd2[1], &a, sizeof(a));
   //  	  else if (req == 'B')
   //  	     write(pd2[1], &b, sizeof(b));
   //
   //  	  read(pd1[0], &req, sizeof(req));
   //    }
   //
   //    _exit(0);
   // }
   // else
   // { // parent
   //    int resp;
   //    char req;
   //
   //    printf("Enter A or B (or X to exit): ");
   //    scanf(" %c", &req);
   //
   //    while (req != 'X')
   //    {
   //  	  write(pd1[1], &req, sizeof(req));
   //  	  read(pd2[0], &resp, sizeof(resp));
   //
   //  	  printf("The value of a is: %d\n", resp);
   //
   //  	  printf("Enter A or B (or X to exit): ");
   //  	  scanf(" %c", &req);
   //    }
   //
   //    write(pd1[1], &req, sizeof(req));
   //    waitpid(-1,NULL,0);
   // }


   return 0;
}

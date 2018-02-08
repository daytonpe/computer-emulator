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
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
// #include <wait.h>

int * loadProgram(const char* file_name);
int * getRandom( );


int main(int argc, char** argv)
{
   //TODO ignore error checks for simplicity

   //Grab the arguments from the CLI
   string file_name = argv[1];
   int timer = atoi(argv[2]);
   cout<<"Program File:    "<<file_name<<'\n';
   cout<<"Interrupt timer: "<<timer<<'\n';

   //Create pipes
   int cpu_to_mem[2];
   int mem_to_cpu[2];
   int result1 = pipe(cpu_to_mem);
   int result2 = pipe(mem_to_cpu);
   if (result1 == -1 || result2 == -1){//pipe failed
     cout << "There has been an error in the pipe..." << '\n';
     exit(1);
   }

   char buf[30];
   int PC;

   //pass txt file into cpu first
   //while not exit (true), switch for the 30 commands
   //cpu requests next instruction
   //incrementing on the PC


   //Fork int CPU and Memory.
   int z = fork();

   if (z<0){
     cout << "There has been an error in the fork..." << '\n';
     exit(-1);
   }

   // Memory Code
   else if (z==0) { //child process code

     //Create Memory Array
     int *mem; //2000 integer entries, 0-999 for the user program, 1000-1999 for system code

     //Load txt file into memory //TODO: should this be in here? or outside?
     mem = loadProgram(file_name.c_str());

     for (size_t i = 0; i < 50; i++) { //TODO: Getting some double numbers and a leading 0
       cout << "Mem Index "<< i <<" : " << mem[i]<< endl;
     }


     //Piped in data
     char command = 'R';
     int address = 3;
     int data = 99;

     int resp;

     read(cpu_to_mem[0], &resp, sizeof(resp));
     cout << "Memory read: "<<  resp <<endl;

     // //read/write functionality
     // switch (command) {
     //   case 'R': //Read function
     //      //return mem[address];
     //      cout << "Read Called" << '\n';
     //   break;
     //   case 'W': //Write function
     //      // mem[address] = data;
     //      cout << "Write Called" << '\n';
     //   break;
     // }


     _exit(0); //terminate this process

   }

   // CPU Code
   else { //parent process code

     //Create Registers
     int PC, SP, IR, AC, X, Y;

     PC = 1414; //test value;


     write(cpu_to_mem[1], &PC, sizeof(PC));



     cout << "CPU process was called" << '\n';
   }

   return 0;
}


// Load program from txt FILE
// Originally wrote in C, converted rest of file to C++
int * loadProgram(const char* file_name)
{
  FILE *ptr_file;
	char buf[1000];
  char ins_str[50];
  static int memory[2000];
  int line = 0;

  // zero out program portion of Array
  // as I was getting weird numbers starting at 436.
  int n = sizeof(memory)/sizeof(memory[0]);
  for (size_t i = 0; i < n; i++) {
    memory[i]=0;
  }

	ptr_file =fopen(file_name,"r");
	if (!ptr_file){
    printf("%s\n", "There has been an error opening your file." );
    return memory;
  }

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
          sscanf(buf, "%d %*s\n", &num);
          memory[line] = num;
          line++;
        }
      }
      else line++;

	fclose(ptr_file);

  for(int j = 0; j < 50; j++) {
      printf("%d", j);
      printf("%s", "\t");
      printf("%d", memory[j]);
      printf("%s", "\n");
  }

  printf("%s", "\nPROGRAM LOADED...\n\n");

 	return memory;
}

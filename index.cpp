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

int * loadProgram(const char* file_name);

int main(int argc, char** argv)
{
   //TODO ignore error checks for simplicity

   //Grab the arguments from the CLI
   string file_name = argv[1];
   int timer = atoi(argv[2]);
   //cout<<"Program File:    "<<file_name<<'\n';
   //cout<<"Interrupt timer: "<<timer<<'\n';

   //Create pipes
   int cpu_to_mem[2];
   int mem_to_cpu[2];
   int result1 = pipe(cpu_to_mem);
   int result2 = pipe(mem_to_cpu);
   if (result1 == -1 || result2 == -1){//pipe failed
     //cout << "There has been an error in the pipe..." << '\n';
     exit(1);
   }

   char buf[30];


   //pass txt file into cpu first
   //while not exit (true), switch for the 30 commands
   //cpu requests next instruction
   //incrementing on the PC


   //Fork int CPU and Memory.
   int z = fork();

   if (z<0){
     //cout << "There has been an error in the fork..." << '\n';
     exit(-1);
   }

   // Memory Code
   //##############################################################################################
   //##############################################################################################
   else if (z==0) { //child process code

     //Create Memory Array
     int *mem; //2000 integer entries, 0-999 for the user program, 1000-1999 for system code

     //Load txt file into memory
     mem = loadProgram(file_name.c_str());

     int PC;
     string save_string;
     int value;
     int address;

     while (true) {
       read(cpu_to_mem[0], &PC, sizeof(PC));
       //write command
       if (PC == -1) { //if we receive a -1 for PC we know we need to write something.
         read(cpu_to_mem[0], &address, sizeof(address)); //second number is the address to which we are writing.
         read(cpu_to_mem[0], &value, sizeof(value)); //third number is the value that we are writing.
         mem[address] = value; //TODO: Need to add a check to make sure we can't write where we aren't supposed to.
       }
       else{ //read command
         int instruc = mem[PC];
         write(mem_to_cpu[1], &instruc, sizeof(instruc));

       }

       // cout << endl;
       // cout << "Stack" << endl;
       // cout <<"1997: "<< mem[1997]<< endl;
       // cout <<"1998: "<< mem[1998]<< endl;
       // cout <<"1999: "<< mem[1999]<< endl;
     }



     _exit(0); //terminate this process
   }

   // CPU Code
   //##############################################################################################
   //##############################################################################################
   else { //parent process code

     //Create Registers
     int PC, SP, IR, AC, X, Y;
     int operand = 0;
     string save_string = "";
     X = 0;
     Y = 0;
     IR = 0;
     AC = 0;
     SP = 2000; //starts at 1999 and works it's way 'upward' -->1998, 1997, etc.
     PC = 0; //initialize PC

     int write_flag = -1;

     int local_reg;

     while (true){

       // fetch next instruction
       write(cpu_to_mem[1], &PC, sizeof(PC));
       read(mem_to_cpu[0], &IR, sizeof(IR));

       // std::cout << "Fetched: " << IR << '\n';
       switch (IR) {

        case 1: //Load Value
          // cout << "1: Load Value " << endl;
          PC++; //since method has operand, increase PC
          write(cpu_to_mem[1], &PC, sizeof(PC)); //ask for the operand
          read(mem_to_cpu[0], &operand, sizeof(operand)); //read the returned operand
          //cout << "OPERAND: " << operand << '\n';
          AC = operand;
          //cout <<"AC: " << AC << endl;
          break;

        case 2: //Load address
          // cout << "2 = Load addr" << endl;
          PC++; //since method has operand, increase PC
          write(cpu_to_mem[1], &PC, sizeof(PC)); //ask for the operand
          read(mem_to_cpu[0], &operand, sizeof(operand)); //fetch operand
          write(cpu_to_mem[1], &operand, sizeof(operand)); //ask for value at mem[operand]
          read(mem_to_cpu[0], &operand, sizeof(operand)); //read the value returned by memory
          AC = operand; //save it to the AC
          //cout <<"AC: " << AC << endl;
          break;

        case 3:
          //cout << "3 = LoadInd addr   " << endl;
          PC++; //since method has operand, increase PC
          write(cpu_to_mem[1], &PC, sizeof(PC)); //ask for the operand
          read(mem_to_cpu[0], &operand, sizeof(operand)); //fetch operand
          // //cout << "operand: " << operand << endl;
          write(cpu_to_mem[1], &operand, sizeof(operand)); //ask for value at mem[operand]
          read(mem_to_cpu[0], &operand, sizeof(operand)); //read the value returned by memory
          // //cout << "operand: " << operand << endl;
          write(cpu_to_mem[1], &operand, sizeof(operand)); //ask AGAIN for value at mem[operand]
          read(mem_to_cpu[0], &operand, sizeof(operand)); //read the value returned by memory
          // //cout << "operand: " << operand << endl;
          AC = operand;
          break;//Load the value from the address found in the given address into the AC

        case 4:
          //cout << "4 = LoadIdxX addr" << endl;
          PC++; //since method has operand, increase PC
          write(cpu_to_mem[1], &PC, sizeof(PC)); //ask for the operand
          read(mem_to_cpu[0], &operand, sizeof(operand)); //fetch operand
          //cout << "operand: " <<operand<< '\n';
          operand+=X;
          // std:://cout << "X: " <<X<< '\n';
          write(cpu_to_mem[1], &operand, sizeof(operand)); //ask for value at mem[operand]
          read(mem_to_cpu[0], &operand, sizeof(operand)); //read the value returned by memory
          AC = operand; //save it to the AC
          // //cout <<"AC: " << AC << endl;
          break;//Load the value at (address+X) into the AC

        case 5:
          //cout << "5 = LoadIdxY addr" << endl;
          PC++; //since method has operand, increase PC
          write(cpu_to_mem[1], &PC, sizeof(PC)); //ask for the operand
          read(mem_to_cpu[0], &operand, sizeof(operand)); //fetch operand
          //cout << "operand: " <<operand<< '\n';
          operand+=Y;
          write(cpu_to_mem[1], &operand, sizeof(operand)); //ask for value at mem[operand]
          read(mem_to_cpu[0], &operand, sizeof(operand)); //read the value returned by memory
          AC = operand; //save it to the AC
          break;//Load the value at (address+Y) into the AC

        case 6: //Load from (Sp+X) into the AC
          //cout << "6 = LoadSpX" << endl;
          operand = SP + X;
          write(cpu_to_mem[1], &operand, sizeof(operand)); //ask for value at mem[operand]
          read(mem_to_cpu[0], &operand, sizeof(operand)); //read the value returned by memory
          AC = operand; //save it to the AC
          break;

        case 7: //Store the value in the AC to the address
          // cout << "7 = Store addr" << endl;
          PC++;
          write(cpu_to_mem[1], &PC, sizeof(PC)); //ask for the operand
          read(mem_to_cpu[0], &operand, sizeof(operand)); //fetch operand
          write(cpu_to_mem[1], &write_flag, sizeof(write_flag)); //send the write flag
          write(cpu_to_mem[1], &operand, sizeof(operand)); //send the address to which we are storing
          write(cpu_to_mem[1], &AC, sizeof(AC)); //send the value we are storing
          break;

        case 8: //AC = random integer from 1 - 100
          AC = rand() % 100 + 1;
          // cout << "8 = Get Random: " << AC << endl;
          // cout << AC << endl;
          break;

        case 9: // Print to screen
          //cout << "9 = Put port" << endl;
          PC++;
          write(cpu_to_mem[1], &PC, sizeof(PC));
          read(mem_to_cpu[0], &operand, sizeof(operand)); //read the value returned by memory
          if (operand == 1) {
            printf("%i", AC );
          }
          if (operand == 2) {
            printf("%c", AC );
          }
          break;

        case 10:
          //cout << "10 = AddX" << endl;
          AC+=X;
          break;//Add the value in X to the AC

        case 11:
          //cout << "11 = AddY" << endl;
          AC+=Y;
          break;//Add the value in Y to the AC

        case 12:
          //cout << "12 = SubX" << endl;
          AC-=X;
          break;//Subtract the value in X from the AC

        case 13:
          //cout << "13 = SubY" << endl;
          AC-=Y;
          break;//Subtract value in Y from the AC

        case 14: //Copy to X
          //cout << "14 = CopyToX" << endl;
          X = AC;
          break;

        case 15: //Copy from X
          //cout << "15 = CopyFromX" << endl;
          AC = X;
          break;

        case 16: //Copy to Y
          //cout << "16 = CopyToY" << endl;
          Y = AC;
          break;

        case 17: //Copy from Y
          //cout << "17 = CopyFromY" << endl;
          AC = Y;
          break;

        case 18: //Copy to SP
          //cout << "18 = CopyToSp" << endl;
          SP = AC;
          break;

        case 19: //Copy from SP
          //cout << "19 = CopyFromSp   " << endl;
          AC = SP;
          break;

        case 20: //Jump to Address
          //cout << "20 = Jump addr" << endl;
          PC++;
          write(cpu_to_mem[1], &PC, sizeof(PC)); //ask for value at mem[operand]
          read(mem_to_cpu[0], &operand, sizeof(operand)); //read the value returned by memory
          PC = operand-1; //set PC to the value retrieved at the operand address minus 1 (due increment after switch statement )
          break;

        case 21: //Jump to address if AC == 0
          //cout << "21 = JumpIfEqual addr" << endl;
          PC++;
          write(cpu_to_mem[1], &PC, sizeof(PC)); //ask for value at mem[operand]
          read(mem_to_cpu[0], &operand, sizeof(operand)); //read the value returned by memory
          // std:://cout << "OPERAND: " << operand << '\n';
          if (AC == 0) {
            // std:://cout << "PC: " << PC << '\n';
            PC = operand-1; //set PC to the value retrieved at the operand address minus 1 (due increment after switch statement )
            // std:://cout << "PC: " << PC << '\n';
          }
          break;

        case 22: //Jump to address if AC != 0
          //cout << "22 = JumpIfNotEqual addr" << endl;
          PC++;
          write(cpu_to_mem[1], &PC, sizeof(PC)); //ask for value at mem[operand]
          read(mem_to_cpu[0], &operand, sizeof(operand)); //read the value returned by Memory
          if (AC != 0) {
            PC = operand-1; //set PC to the value retrieved at the operand address minus 1 (due increment after switch statement )
            // std:://cout << "PC: " << PC << '\n';
          }
          break;

        case 23: //Push return address onto stack, jump to the address
          // cout << "23 = Call addr" << endl;

          //Grab the operand so that we can access it later
          PC++;
          write(cpu_to_mem[1], &PC, sizeof(PC)); //ask for value at mem[operand]
          read(mem_to_cpu[0], &operand, sizeof(operand)); //read the value returned by memory

          //push return address onto stack
          SP--;
          PC++; //increment again because return address will be 2 after current addess due to param.
          write(cpu_to_mem[1], &write_flag, sizeof(write_flag)); //send the write flag
          write(cpu_to_mem[1], &SP, sizeof(SP)); //store it at the stack pointer (address where we are storing)
          write(cpu_to_mem[1], &PC, sizeof(PC)); //send the return address (value we are storing)

          PC = operand-1; //finally, set PC to the value retrieved at the operand address minus 1 (due increment after switch statement )
          // cout << "SP: " << SP << endl;
          // cout << "PC: " << PC << '\n';
          // cout << "SP: " << SP << endl;

          break;

        case 24: //Pop return address from the stack, jump to the address
          // cout << "24 = Ret " << endl;

          //pop return address from stack
          write(cpu_to_mem[1], &SP, sizeof(SP)); //ask memory "what's at SP?"

          //jump to the address
          read(mem_to_cpu[0], &PC, sizeof(PC)); //set memory's reply to PC
          PC--; //account for PC++ at end
          SP++;
          // cout << "SP: " << SP << endl;
          break;

        case 25: //Increment X
          //cout << "25 = IncX " << endl;
          X++;
          break;

        case 26: //Decrement X
          //cout << "26 = DecX " << endl;
          X--;
          break;

        case 27:
          //cout << "27 = Push" << endl;
          break;

        case 28:
          //cout << "28 = Pop" << endl;
          break;

        case 29:
          //cout << "29 = Int " << endl;
          break;

        case 30:
          //cout << "30 = IRet" << endl;
          break;

        case 50:
          //cout << "50 = End	" << endl;
          _exit(0);
          break;

        default:
          cout << "ERROR: NOT A COMMAND" << '\n';
       }
       PC++;
       // std:://cout << "Post Statement PC: " << PC << '\n';
       //cout << endl;
     }
   }

   return 0;
}

//Helper Functions
//##############################################################################################
//##############################################################################################

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

  // for(int j = 0; j < 50; j++) {
  //     printf("%d", j);
  //     printf("%s", "\t");
  //     printf("%d", memory[j]);
  //     printf("%s", "\n");
  // }

  printf("%s", "\nPROGRAM LOADED...\n\n");

 	return memory;
}

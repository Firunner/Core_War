#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#include "get_RedCode.c"
#include "is_correct.c"
#include "conversions.h"
#include "struct_def.h"

#define CORE_S 10

int type_instruction_from_int (int instruction){
    int b = (0x1<<28)|(0x1<<29)|(0x1<<30)|(0x1<<31);
    int dat = 0;
    int jmp = (0x1<<30);
    int type = (b & instruction);
    // printf("%d\n",type);
    if ((type == dat) || (type & jmp))
        return 1;
    else
        return 2;
}
instruction_A_B int_to_instructionA_B(int instruction){
    
    instruction_A_B instr;
    instr.B = instruction & (0x7FF);
    instr.A = (instruction >> 12) & (0x7FF);
    int sign_A = (instruction >> 23) & 1;
    int sign_B = (instruction >> 11) & 1;
    int mB = (instruction >> 24) & (0x3);
    int mA = (instruction >> 26) & (0x3);
    int ty = (instruction >> 28) & (0xF);
    
    if (sign_A == 1)
        instr.A = -1 * instr.A;
    if (sign_B == 1)
        instr.B = -1 * instr.B;
    
    switch (mB) {
        case 0 :
            instr.modeB = '#';
            break;
        case 1 :
	  break;
        case 2 :
            instr.modeB = '@';
            break;
    }
    switch (mA) {
        case 0 :
            instr.modeA = '#';
            break;
        case 1 :
	  break;
        case 2 :
            instr.modeA = '@';
            break;
    }
   switch (ty) {
        case 0 :
            strcpy(instr.type, "DAT");
            break;
        case 1 :
            strcpy(instr.type, "MOV");
            break;
        case 2 :
            strcpy(instr.type, "ADD");
            break;
        case 3 :
            strcpy(instr.type, "SUB");
            break;
        case 4 :
            strcpy(instr.type, "JMP");
            break;
        case 5 :
            strcpy(instr.type, "JMZ");
            break;
        case 6 :
            strcpy(instr.type, "DJZ");
            break;
        case 7 :
            strcpy(instr.type, "CMP");
            break;

    }
    
    return instr;
}
instruction_B int_to_instruction_B(int instruction){
    
    instruction_B instr;
    instr.B = instruction & (0x7FF);
    int sign_B = (instruction >> 11) & 1;
    int mB = (instruction >> 24) & (0x3);
    int ty = (instruction >> 28) & (0xF);
    
    if (sign_B == 1)
        instr.B = -1 * instr.B;
    
    switch (mB) {
        case 0 :
            instr.modeB = '#';
            break;
        case 1 :            
            break;
        case 2 :
            instr.modeB = '@';
            break;
    }
    switch (ty) {
        case 0 :
            strcpy(instr.type, "DAT");
            break;
        case 1 :
            strcpy(instr.type, "MOV");
            break;
        case 2 :
            strcpy(instr.type, "ADD");
            break;
        case 3 :
            strcpy(instr.type, "SUB");
            break;
        case 4 :
            strcpy(instr.type, "JMP");
            break;
        case 5 :
            strcpy(instr.type, "JMZ");
            break;
        case 6 :
            strcpy(instr.type, "DJZ");
            break;
        case 7 :
            strcpy(instr.type, "CMP");
            break;

    }    
    return instr;
}




int main(int argc, char ** argv){
  
  FILE* my_RedCode = NULL;
  
  int i = 0, position1, position2, nb_lines;  
  int Core[CORE_S];  
  
  my_RedCode = fopen("Red_Code.txt", "r");

  nb_lines = nb_instructions_in_RedCode(my_RedCode);
  
  fseek(my_RedCode, 0, SEEK_SET);  

  char **my_lines_of_RedCode = (char **)malloc(sizeof(char*) * nb_lines);
  char *lines = (char *) malloc(sizeof(char) * 40 * nb_lines);
  for (int j = 0 ; j < nb_lines ; j++)
    my_lines_of_RedCode[j] = &lines[j*40];  
  
  for (int j = 0 ; j < nb_lines ; j++)
    get_RedCode(my_RedCode, my_lines_of_RedCode[j]);

 
  position1 = rand() % CORE_S;
  position2 = ((rand() % (CORE_S - nb_lines*2)) + position1 + nb_lines) % CORE_S;

  
  
  for(int j = 0 ; j < nb_lines ; j++){
    if (is_instruction_correct(my_lines_of_RedCode[j]) == 1)
      Core[(position1+j)%CORE_S] = get_instruction_A_Binstruction_to_int_B(my_lines_of_RedCode[j]);
    else if (is_instruction_correct(my_lines_of_RedCode[j]) == 2)
      Core[(position1+j)%CORE_S] = instruction_to_int_A_B(my_lines_of_RedCode[j]);
    else     
      Core[(position1+j)%CORE_S] = -1;
  }
  
  for (int j = 0 ; j < nb_lines ; j++){
    if (Core[j] != -1){
      int k = type_instruction_from_int(Core[j]);
      if (k == 1)
	printf("instruction n°%d est de type B\n", j);
      else if(k == 2)
	     printf("instruction n°%d est de type A_B\n", j);
    }
  }
 
  
  

  


  return 0;
}

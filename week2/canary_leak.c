#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
// compile : gcc -fno-stack-protector -no-pie -z execstack -o canary_leak canary_leak.c
char canary_saved[8];

void get_random_byte(char *canary) {

  for(int i =0;i<8;i++){
    canary[i] = (char)(rand() % (0x7e - 0x21 + 1) + 0x21);
  }
	return;
}

int main(){
  
  char canary[8];
  char memo[0x10];
  char title[0x10];
  int target = 0;
  srand(time(NULL));
  setvbuf(stdout, 0, 2, 0);
  get_random_byte(canary_saved);
  memcpy(canary, canary_saved, 8);

  printf("Input memo title\n");
  read(0, title, 0x40);
  printf("[Title] %s\n", title);
  printf("Input memo\n");
  gets(memo);
  printf("[Memo] %s\n", memo);

  if(strncmp(canary_saved, canary, 8) == 0){
    if(target  == 0x72616562){
      system("cat /home/canary_leak/flag");
    } else {
      printf("Good Canary test passed!\n make target 0x72616562\n current target value: %d", target);
    }
  } else {
    printf("NO.. canary changed! Try again\n");
    exit(0);
  }

}
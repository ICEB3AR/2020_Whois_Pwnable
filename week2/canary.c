#include <stdio.h>


void hello(){
  char hi[4] = "HI!\n";
  printf("%s\n", hi);
}
void bye(){
  char bye[5] = "BYE!\n";
  printf("%s\n", bye);
}
int main(){

  char buf[0x10];
  hello();
  scanf("%s\n", buf);
  bye();
}
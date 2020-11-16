#include <stdio.h>

void hello(){
	char hello[0x10] = "Hello_World!\n";
	printf("%s", hello);
}
void bye(){
	char bye[0x10] = "Bye_Bye!\n";
	printf("%s", bye);
}

int main(){
	hello();
	bye();
}

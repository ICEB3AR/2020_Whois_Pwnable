#include <stdio.h>
#include <string.h>

#define STACK_SIZE 0x38
#define CANARY_OFFSET STACK_SIZE
#define SFP_OFFSET STACK_SIZE+0x8
#define RET_OFFSET STACK_SIZE+0x8+0x8


/**
* compile : gcc -o bof_tutorial bof_tutorial.c -no-pie -z execstack
* shellcode (23 bytes): 31f648bb2f62696e2f2f73685653545f6a3b5831d20f05
*/
void welcome(){
    printf(
    " ____   ____  ______ \n"
    "|  _ \\ / __ \\|  ____|\n"
    "| |_) | |  | | |__\n" 
    "|  _ <| |  | |  __|\n"  
    "| |_) | |__| | |\n"     
    "|____/ \\____/|_|\n");
    printf("\n[Buffer Overflow Tutorial]\n");
    printf("\n- Made by exd0tpy -\n\n");
    printf("\nshellcode (23 bytes): 31f648bb2f62696e2f2f73685653545f6a3b5831d20f05\n\n");
}
void printAscii(char *dest){
    for(int i=0;i<8;i++){
        if(dest[i] == '\n') printf("\\n");
        else printf("%c", dest[i]);
    }
}
void print8(char *dest, int isStack){
    printf("\n| ");
    for(int i=0;i<8;i++){
        printf("%02x", dest[i]&0xff);
    }

    if(isStack){
        printf(" |\t");
        printAscii(dest);
        printf("\n");
    } else {
        printf(" |");
    }
}
__uint8_t hex(char ch) {
    __uint8_t r = (ch > 57) ? (ch - 55) : (ch - 48);
    return r & 0x0F;
}

int to_byte_array(const char *in, size_t in_size, __uint8_t *out) {
    int count = 0;
    if (in_size % 2) {
        while (*in && out) {
            *out = hex(*in++);
            if (!*in)
                return count;
            *out = (*out << 4) | hex(*in++);
            *out++;
            count++;
        }
        return count;
    } else {
        while (*in && out) {
            *out++ = (hex(*in++) << 4) | hex(*in++);
            count++;
        }
        return count;
    }
}
void printLine(int offset){
    if (offset == -1) printf("+------------------+\t");
    else printf("+------------------+ 0x%x", offset);
}

void printStack(char *stack){
    unsigned long long canary = *(unsigned long long *)(stack+CANARY_OFFSET);
    unsigned long long sfp = *(unsigned long long *)(stack+SFP_OFFSET);
    unsigned long long ret = *(unsigned long long *)(stack+RET_OFFSET);

    printf("\n[STACK]\nSize: %d bytes\nAddress: %p\n", STACK_SIZE, stack);
    for(int i =0;i<STACK_SIZE/8;i++){
        printLine(i*8);
        print8(stack+8*i, 1);
    }

    printLine(STACK_SIZE);
    print8(stack + STACK_SIZE, 0);
    printf("\t  <-- canary\n");
    printLine(STACK_SIZE+0x8);
    print8(stack + STACK_SIZE + 0x8, 0);
    printf("\t  <-- rbp\n");
    printLine(STACK_SIZE+0x10);
    print8(stack + STACK_SIZE + 0x10, 0);
    printf("\t  <-- return address\n");
    printLine(-1);
}
void getHexInput(char *stack){
    char tmp[STACK_SIZE*4] = {0,};
    printf("[*] ex) 41424344 -> \\x41\\x42\\x43\\x44\n>>");
    fflush(stdout);
    int size = read(0, tmp, STACK_SIZE*4);
    if (tmp[size-1] == '\n') tmp[size-1] = '\x00';
    to_byte_array(tmp, size, stack);
}
int getInput(char *stack){
    printf("\n\n[*] If you want input hex value, input \"hex\"\n[*] If you want return, input \"ret\"\n[*] If you want print stack, input \"print\"\nWrite something to buf\n>>");
    fflush(stdout);
    char tmp[STACK_SIZE*2] = {0,};
    int size = read(0, tmp, STACK_SIZE*2);
    if(!strncmp(tmp, "ret",3)) return 0;
    else if(!strncmp(tmp, "hex", 3)) {
        getHexInput(stack);
    } else if(!strncmp(tmp, "print", 5)) {
        printf("Contents: %s\n", stack);
    } else{
        memcpy(stack, tmp, size);
        printf("\n\n[*] Input %d byte\n", size);
    }
    return 1;
}

int main(){
    char stack[STACK_SIZE] = {0,};
    welcome();
    while(1){
        printStack(stack);
        if(!getInput(stack)) break;
    }
}

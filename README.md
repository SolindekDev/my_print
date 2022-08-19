# My Print
Just a terrible print implementation in C.
## Example use
```c
int main(int argc, char** argv) {
    print("Normal print\n");
    print("Precent sign: %%\n");
    print("Int: %d\n", 310938);
    print("Octals: 0o%o\n", 310938);
    print("Hexadecimal: 0x%x\n", 310938);    
    print("Char: %c\n", 'A');
    print("String: %s\n", "Hello!");
    print("Float: %f\n", 3.14);
    print("Unsigned int: %u\n", 20000);
    
    return 0;
}
```
## Only supports linux machines
This repository do not support window because we use a linux syscall to write into console and program use lots of linux headers.
```c
void syscall_write_char(char c) {
    char buf[2] = {0};
    strncat(buf, &c, 1);
    
    syscall(SYS_write, 1, buf, strlen(buf)); 
}
```
## DON'T USE IT
Don't use this print because it suck.

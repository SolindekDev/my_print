#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

void reverse(char* s) {
    int c, i, j;
    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int _num, char* _str, int _base) {
    int i = 0;
    bool is_neg = false;
    
    if (_num == 0) { 
        _str[i++] = '0';
        _str[i] = '\0';
        return;
    } 
    if (_num < 0 && _base == 10) {
        is_neg = true;
        _num = -_num;
    }
    while (_num != 0) {
        int rem = _num % _base;
        _str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        _num = _num / _base;
    }
    if (is_neg)
        _str[i++] = '-';
    _str[i] = '\0';
    reverse(_str);
}

void syscall_write_char(char c) {
    char buf[2] = {0};
    strncat(buf, &c, 1);
    
    syscall(SYS_write, 1, buf, strlen(buf)); 
}

void print(char* _buf, ...) {
    va_list arg;
    va_start(arg, _buf);
    
    int format_int;
    unsigned int format_unsigned;
    long format_address;
    char format_char;
    char* format_str;
    double format_float;
    char bufprint[1024];
    
    for (int i = 0; i < strlen(_buf); i++) {
        char c = _buf[i];
        
        if (c == '%') {
            i++;
            c = _buf[i];
            
            switch (c) {
                case '%':
                    syscall_write_char('%');
                    break;
                case 'd': case 'i': case 'D': case 'I': case 'o': case 'O': case 'x': case 'X': case 'h': case 'H':
                    format_int = va_arg(arg, int); 
                    if (format_int < 0) {
                        syscall_write_char('-');
                        format_int *= -1;
                    }
                    
                    if (c == 'd' || c == 'i' || c == 'D' || c == 'I') // Decimal
                        itoa(format_int, bufprint, 10);
                    else if (c == 'o' || c == 'O') // Octals
                        itoa(format_int, bufprint, 8);
                    else if (c == 'x' || c == 'X' || c == 'h' || c == 'H') // Hexadecimal
                        itoa(format_int, bufprint, 16);
                    
                    for (int i = 0; bufprint[i] != '\0'; i++)
                        syscall_write_char(bufprint[i]);
                    break;
                case 'c': case 'C':
                    format_char = va_arg(arg, int);
                    syscall_write_char((char)format_char);
                    break;
                case 's': case 'S':
                    format_str = va_arg(arg, char*);
                    for (int i2 = 0; i2 < strlen(format_str); i2++) 
                        syscall_write_char(format_str[i2]);
                    break;
                case 'f': case 'F':
                    format_float = va_arg(arg, double);
                    sprintf(bufprint, "%f", format_float);
                    for (int i = 0; bufprint[i] != '\0'; i++)
                        syscall_write_char(bufprint[i]);
                    break;
                case 'u': case 'U':
                    format_unsigned = va_arg(arg, unsigned int);
                    sprintf(bufprint, "%u", format_unsigned);
                    for (int i = 0; bufprint[i] != '\0'; i++)
                        syscall_write_char(bufprint[i]);
                    break;
                case 'p': case 'P':
                    syscall_write_char('\n');
                    syscall_write_char('%');
                    print("p is not implemented into print func\n");
                    exit(1);
                    break;
                default:
                    break;
            }
        }
        else 
            syscall_write_char(c);
    }
    va_end(arg);
}

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

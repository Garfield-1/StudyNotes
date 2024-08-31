#include <stdio.h>
#include <stdlib.h> 
#include <execinfo.h>

void print_call_stack() {
    void *call_stack[10];
    int size = backtrace(call_stack, 10);
    char **symbols = backtrace_symbols(call_stack, size);
    if (symbols == NULL) {
        perror("backtrace_symbols");
        return;
    }
    for (int i = 0; i < size; ++i) {
        if (symbols[i] != NULL) { // 检查字符串是否为空指针
            printf("%s\n", symbols[i]);
        } else {
            printf("Unknown function\n"); // 如果字符串为空指针，打印未知函数
        }
    }
    free(symbols);
}

void func3() {
    print_call_stack();
}

void func2() {
    func3();
}

void func1() {
    func2();
}

int main() {
    func1();
    return 0;
}
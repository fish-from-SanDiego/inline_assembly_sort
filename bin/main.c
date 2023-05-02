#include <stdio.h>

int main() {
    int array[1000];
    int n = 0;

    FILE* fp = fopen("input.txt", "r");

    while (fscanf(fp, "%d", array + n) != EOF) {
        n++;
    }

    fclose(fp);
    int swapped = 0;

    __asm__(
        "movl $0x1, %0\n\t"
        "sort_begin:\n\t"
        "cmpl $0x0, %0\n\t" // no swaps(swapped = 0) -> arr sorted
        "je sort_end\n\t"
        "movl $0x0, %0\n\t"
        "movq $0, %%rsi\n\t" // i = 0
        "swap_cycle:\n\t"  
        "incq %%rsi\n\t" // ++i
        "cmpq %%rax, %%rsi\n\t"  // if i >= n exit loop
        "jge swap_cycle_end\n\t" 
        "movl (%%rbx, %%rsi, 0x4),%%ecx\n\t"
        "movl -0x4(%%rbx, %%rsi, 0x4), %%edx\n\t"
        "cmpl %%edx, %%ecx\n\t"  // if a[i] > a[i - 1], don't swap
        "jge dont_swap\n\t"
        "xorl %%edx, %%ecx\n\t" // swap a[i], a[i - 1]
        "xorl %%ecx, %%edx\n\t"
        "xorl %%edx, %%ecx\n\t"
        "movl %%ecx, (%%rbx, %%rsi, 0x4)\n\t"      
        "movl %%edx, -0x4(%%rbx, %%rsi, 0x4)\n\t"
        "movl $0x1, %0\n\t" // swapped = 1
        "dont_swap:\n\t"   
        "jmp swap_cycle\n\t"  // back to start of the loop
        "swap_cycle_end:\n\t"
        "jmp sort_begin\n\t" // begin another loop
        "sort_end:\n\t" // sorted
        : "+r"(swapped)
        : "b"(array), "a"(n)
        : "memory", "cc", "rsi", "ecx", "edx"
    );
    fp = fopen("output.txt", "w");

    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d\n", array[i]);
    }

    fclose(fp);

    return 0;
}

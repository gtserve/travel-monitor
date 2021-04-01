#ifndef HW_SYSPRO_BIT_ARRAY_H
#define HW_SYSPRO_BIT_ARRAY_H

// You can change the type of number held inside the array.
#define NUM_TYPE int
#define BIT_SIZE (sizeof(NUM_TYPE) * 8)

// Operations (as macros)
#define SetBit(A, k)    (A[(k/BIT_SIZE)] |= (1 << k % BIT_SIZE))
#define ClearBit(A, k)  (A[(k/BIT_SIZE)] &= ~(1 << k % BIT_SIZE))
#define TestBit(A, k)   (A[(k/BIT_SIZE)] & (1 << k % BIT_SIZE))

#endif //HW_SYSPRO_BIT_ARRAY_H

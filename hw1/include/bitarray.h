#ifndef HW_SYSPRO_BITARRAY_H
#define HW_SYSPRO_BITARRAY_H

// You can change the type of number held inside the array.
#define BAR_TYPE int
#define BAR_TYPE_BITS (sizeof(BAR_TYPE) * 8)

// Operations (as macros)
#define SetBit(A, b) ((A)[((b)/BAR_TYPE_BITS)] |= (1 << ((b) % BAR_TYPE_BITS)))
#define ClearBit(A, b) ((A)[((b)/BAR_TYPE_BITS)] &= ~(1 << ((b) % BAR_TYPE_BITS)))
#define TestBit(A, b) ((A)[((b)/BAR_TYPE_BITS)] & (1 << ((b) % BAR_TYPE_BITS)))

#endif //HW_SYSPRO_BITARRAY_H

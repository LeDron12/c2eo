// Вычиление числа фибоначчи
// Использование глобальных переменных

#include <stdio.h>

typedef long long i64;

i64 p1 = 0;
i64 p2 = 1;
i64 arg;
i64 result;
i64 i;

void fibo() {
  i = 1;
  while (i < arg) {
    result = p1 + p2;
    p1 = p2;
    p2 = result;
    i++;
  }
}

int main() {
  arg = 10;
  fibo();
  printf("fibo(%lld) = %lld\n", arg, result);
  return 0;
}
/*
+package c2eo.examples.fibonacci1

+alias org.eolang.gray.cage
+alias c2eo.system.ram
+alias c2eo.system.address
+alias c2eo.stdio.printf
+alias c2eo.coperators.while
+alias c2eo.coperators.write
+alias c2eo.coperators.get
+alias c2eo.coperators.add
+alias c2eo.coperators.less
+alias c2eo.coperators.add-int64
+alias c2eo.coperators.as-int64

[args...] > global

  * (* cage cage cage) > allocator
  memory > allocator-index

  ram 40 > g-ram
  ram 0 > s-ram

  address g-ram 0 > p1
  address g-ram 8 > p2
  address g-ram 16 > arg
  address g-ram 24 > result
  address g-ram 32 > i

  [] > eo-init
    seq > @
      write (get (get allocator 0) 0) g-ram
      write (g-ram.index) 0
      write (get (get allocator 0) 1) s-ram
      write (s-ram.index) 1
      write allocator-index 2
      write p1 0
      write p2 1
      write arg 0
      write result 0
      write i 0
      TRUE

  [] > fibo
    ram 0 > l-ram
    seq > @
      write (get (get allocator 0) allocator-index) l-ram
      write allocator-index (add allocator-index 1)
      write i 1
      while
        less (as-int64 i) (as-int64 arg)
        seq
          write result (add-int64 p1 p2)
          write p1 (as-int64 p2)
          write p2 (as-int64 result)
          write i (add 1 (as-int64 i))
          TRUE

  [args] > main
    seq > @
      write arg 10
      fibo
      printf "fibo(%d) = %d\n" (as-int64 arg) (as-int64 result)
      TRUE

  seq > @
    eo-init
    main args
    TRUE
*/
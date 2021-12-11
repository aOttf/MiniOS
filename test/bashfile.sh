gcc -c kernel.c cpu.c ram.c pcb.c shell.c shellmemory.c interpreter.c memorymanager.c ReadyQueue.c straux.c

gcc -o mykernel kernel.o cpu.o ram.o pcb.o shell.o shellmemory.o interpreter.o memorymanager.o ReadyQueue.o straux.o


rm *.o

exit 0
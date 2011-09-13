objects = queue.o semaphore.o threadlib.o unix_routines.o

libmythread.a: $(objects)
	ar rcs libmythread.a $(objects)

$(objects): mythread.h threadlib.h

.PHONY: clean 
clean:
	rm mythread.a $(objects) 

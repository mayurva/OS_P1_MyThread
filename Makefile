includes = mythread.h threadlib.h
objects = queue.o semaphore.o threadlib.o unix_routines.o
library = mythread.a

$(library): $(objects)
	ar rcs $(library) $(objects)

$(objects): $(includes)

.PHONY: clean 
clean:
	rm -f $(library) $(objects)


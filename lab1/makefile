cc=gcc -Wall
N_CHILDREN=4

1-loop: 1.c
	${cc} -o 1-loop.o 1.c -D LOOP=1 -D N_CHILDREN=${N_CHILDREN}
	@echo "##################################################"
	@./1-loop.o
	@echo "##################################################"

1-recursion: 1.c
	${cc} -o 1-recursion.o 1.c -D RECURSION=1 -D N_CHILDREN=${N_CHILDREN}
	@echo "##################################################"
	@./1-recursion.o
	@echo "##################################################"

2: 2.c
	${cc} -o 2.o 2.c -D N_CHILDREN=${N_CHILDREN}
	@echo "##################################################"
	@./2.o
	@echo "##################################################"

3: 3.c
	${cc} -o 3.o 3.c -D N_CHILDREN=${N_CHILDREN}
	@echo "##################################################"
	@./3.o 3-test-0 3-test-1 3-test-2 3-test-3
	@echo "##################################################"

4: 4.c
	${cc} -o 4.o 4.c -D N_CHILDREN=${N_CHILDREN}
	@echo "##################################################"
	@./4.o
	@echo "##################################################"

clean:
	rm ./*.o

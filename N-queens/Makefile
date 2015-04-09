all: comp link

comp: n-queen.c
	g++ -o n-queen.o -c n-queen.c -I$(AMDAPPSDKROOT)/include
link: n-queen.o
	g++ -o n-queen n-queen.o -lOpenCL -L$(AMDAPPSDKROOT)/lib/x86_64
comp2:
	g++ -o n-queen.o -c n-queen.c
link2:
	g++ -o n-queen n-queen.o 
clean:
	rm -rf *.o
run:
	sudo ./n-queen

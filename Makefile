all: dialog.o main.o tree.o visualization.o debug.o
	gcc -o main -lreadline dialog.o main.o input/read.o tree.o visualization.o debug.o
dialog.o: dialog/dialog.c
	gcc -c -o dialog.o dialog/dialog.c
main.o: main.c
	gcc -c -o main.o main.c
tree.o: tree/tree.c
	gcc -c -o tree.o tree/tree.c
visualization.o: visualization/visualization.c
	gcc -c -o visualization.o visualization/visualization.c
debug.o: debug/debug.c
	gcc -c -o debug.o debug/debug.c
clean:
	rm -rf main
	rm -rf time
	rm -rf *.dot *.o

run:
	gcc `pkg-config --cflags gtk4` tfv1.c `pkg-config --libs gtk4`
	./a.out
# -*- MakeFile -*-

bin: produtor_consumidor.c
	gcc $< -o $@ -lpthread

run:
	./bin

clean:
	rm bin

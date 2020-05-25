# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# opcje optymalizacji:
# wersja do debugowania
#OPT = -g -DEBUG 
# wersja zoptymalizowana do mierzenia czasu
OPT = -O3

# pliki naglowkowe
INC = -I../pomiar_czasu

# biblioteki
LIB = -L../pomiar_czasu -lpomiar_czasu -lm -lpthread

zad2: zad2.o 
	$(LOADER) $(OPT) zad2.o -o zad2 $(LIB)

zad2.o: zad2.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) zad2.c $(INC) 

watki: watki.o 
	$(LOADER) $(OPT) watki.o -o watki $(LIB)

watki.o: watki.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) watki.c $(INC)

zad4: zad4.o 
	$(LOADER) $(OPT) zad4.o -o zad4 $(LIB)

zad4.o: zad4.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) zad4.c $(INC) 

zad6: zad6.o 
	$(LOADER) $(OPT) zad6.o -o zad6 $(LIB)

zad6.o: zad6.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) zad6.c $(INC) 

clean:
	rm -f *.o

COMPILER        =       gcc
CFLAGS          =       -Ofast
LFLAGS          =       -lm
OBJECTS         =       main.o epi.o 
OBJECTSTEST		=		test.o mkGr.o
INCLUDES        =       mkGr.h myFunctions.h aStar.h

main:           main.o mkGr.o aStar.o myFunctions.o
		$(COMPILER) $(CFLAGS) -o main main.o mkGr.o aStar.o myFunctions.o $(LFLAGS)

main.o: main.c $(INCLUDES)
		$(COMPILER) $(CFLAGS) -c main.c $(LFLAGS)

mkGr.o:			mkGr.c $(INCLUDES)
		$(COMPILER) $(CFLAGS) -c mkGr.c $(LFLAGS)

aStar.o: aStar.c $(INCLUDES)
		$(COMPILER) $(CFLAGS) -c aStar.c $(LFLAGS)


myFunctions.o: myFunctions.c $(INCLUDES)
		$(COMPILER) $(CFLAGS) -c myFunctions.c $(LFLAGS)

test:			$(OBJECTSTEST)
		$(COMPILER) $(CFLAGS) -o test $(OBJECTSTEST) $(LFLAGS)
runTest:		test
		perf stat ./test spain.csv bla \| 3 23895681 1417363 79858

test.o:		$(INCLUDES) test.c
		$(COMPILER) $(CFLAGS) -c test.c $(LFLAGS)

runMain:		main
		perf stat ./main bla 240949599 195977239

clean:
		rm -f *.o *~

realclean:	clean
		rm -f main

tclean: clean
		rm -f test

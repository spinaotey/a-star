COMPILER        =       gcc
CFLAGS          =       -Ofast
LFLAGS          =       -lm
OBJECTS         =       main.o epi.o 
OBJECTSTEST		=		makeGraph.o mkGr.o
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

makeGraph:			$(OBJECTSTEST)
		$(COMPILER) $(CFLAGS) -o makeGraph $(OBJECTSTEST) $(LFLAGS)
runMakeGraph:		makeGraph
		perf stat ./makeGraph spain.csv graph.bin \| 3 23895681 1417363 79858

makeGraph.o:		$(INCLUDES) makeGraph.c
		$(COMPILER) $(CFLAGS) -c makeGraph.c $(LFLAGS)

runMain:		main
		perf stat ./main graph.bin 240949599 195977239

clean:
		rm -f *.o *~

realclean:	clean
		rm -f main

tclean: clean
		rm -f test

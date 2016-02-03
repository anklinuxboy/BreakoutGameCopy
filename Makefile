CC=g++ -std=c++11
CFLAGS=-g

FILE=breakout

$(FILE): $(FILE).cpp 
	$(CC) $(CFLAGS) -o $(FILE) $^ StanfordCPPLib/*.cpp 

clean:
	rm -f *.o core $(FILE)

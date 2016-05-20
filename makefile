all: $(addprefix build/, app.o)
	g++ $^ -o bin/app

build/%.o: %.cpp
	g++ -std=c++11 -c $^ -o $@

clean:
	rm build/*
	rm bin/*


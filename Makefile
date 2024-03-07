CXX = g++
CXXFLAGS = -I/opt/homebrew/Cellar/sfml/2.6.1/include
LDFLAGS = -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system

out: main.cpp
    $(CXX) main.cpp -o out $(CXXFLAGS) $(LDFLAGS)


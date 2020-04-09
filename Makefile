MKDIR=	mkdir -p

RM=		rm -f

SRC=	Pixelarray.cpp \
		Window.cpp \
		Loop.cpp

OBJ=	Pixelarray.o \
		Window.o \
		Loop.o

NAME=	libPinguin.so

HEADER=	Pinguin.h


all: install fclean

compile:
	g++ -c -fPIC $(SRC)

build: compile
	g++ $(OBJ) -shared -o $(NAME)

install: build
	$(MKDIR) /usr/include/Pinguin
	cp $(NAME) /usr/lib/x86_64-linux-gnu/
	cp $(HEADER) /usr/include/Pinguin/

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)
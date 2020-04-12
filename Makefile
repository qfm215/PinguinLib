MKDIR=	mkdir -p

RM=		rm -f

RMDIR=	rm -rf

SRC=	Pixelarray.cpp \
		RWindow.cpp \
		Loop.cpp

OBJ=	Pixelarray.o \
		RWindow.o \
		Loop.o

NAME=	libPinguin.so

HEADER=	Pinguin.h


all: install fclean

compile:
	g++ -c -fPIC $(SRC) -W -Wall -Wextra

build: compile
	g++ $(OBJ) -shared -o $(NAME)

install: build
	cp $(NAME) /usr/lib/x86_64-linux-gnu/
	$(MKDIR) /usr/include/Pinguin
	cp $(HEADER) /usr/include/Pinguin/

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

uninstall:
	$(RMDIR) /usr/include/Pinguin
	$(RM) /usr/lib/x86_64-linux-gnu/$(NAME)
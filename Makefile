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

ZSHCFG=		$(HOME)/.zshrc

BASHCFG=	$(HOME)/.bashrc

LIBRARYEXPORT=	export LD_LIBRARY_PATH=$(HOME)/.pinguinlib/lib:$$LD_LIBRARY_PATH


all: install clean

compile:
	g++ -c -fPIC $(SRC)

build: compile
	g++ $(OBJ) -shared -o $(NAME)

install: build
	$(MKDIR) $(HOME)/.pinguinlib/header
	$(MKDIR) $(HOME)/.pinguinlib/lib
	cp $(NAME) $(HOME)/.pinguinlib/lib/
	cp $(HEADER) $(HOME)/.pinguinlib/header/
ifneq ($(wildcard $(ZSHCFG)),)
	grep -qxF 'export LD_LIBRARY_PATH=$(HOME)/.pinguinlib/lib:$$LD_LIBRARY_PATH' $(ZSHCFG) || echo 'export LD_LIBRARY_PATH=$(HOME)/.pinguinlib/lib:$$LD_LIBRARY_PATH' >> $(ZSHCFG)
endif
ifneq ($(wildcard $(BASHCFG)),)
	grep -qxF '$(LIBRARYEXPORT)' $(BASHCFG) || echo '$(LIBRARYEXPORT)' >> $(BASHCFG)
endif

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)
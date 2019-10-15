MKDIR=	mkdir -p

RM=		rm -f

SRC=	Pixelarray.cpp \
		Window.cpp \
		Loop.cpp

OBJ=	Pixelarray.o \
		Window.o \
		Loop.o

NAME=	libPinguin.so

ZSHCFG=		$(HOME)/.zshrc

BASHCFG=	$(HOME)/.bashrc


all: install clean

compile:
	g++ -c -fPIC $(SRC)

build: compile
	g++ $(OBJ) -shared -o $(NAME)

install: build
	$(MKDIR) $(HOME)/.pinguinlib
	mv $(NAME) $(HOME)/.pinguinlib/
ifneq ($(wildcard $(ZSHCFG)),)
	grep -qxF 'export LD_LIBRARY_PATH=$(HOME)/.pinguinlib:$$LD_LIBRARY_PATH' $(ZSHCFG) || echo 'export LD_LIBRARY_PATH=$(HOME)/.pinguinlib:$$LD_LIBRARY_PATH' >> $(ZSHCFG)
endif
ifneq ($(wildcard $(BASHCFG)),)
	grep -qxF 'export LD_LIBRARY_PATH=$(HOME)/.pinguinlib:$$LD_LIBRARY_PATH' $(BASHCFG) || echo 'export LD_LIBRARY_PATH=$(HOME)/.pinguinlib:$$LD_LIBRARY_PATH' >> $(BASHCFG)
endif

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)
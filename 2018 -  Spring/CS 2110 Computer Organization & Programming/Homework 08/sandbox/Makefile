CC = gcc-5
PROGNAME = sandbox
SRC = sandbox.c graphics.c qdbmp.c azul.c tv_tester.c
OBJ = $(SRC:%.c=%.o)
CFLAGS = -Wall -Werror -std=c99 -pedantic -Wextra -fmax-errors=10
LFLAGS =

sandbox : $(OBJ)
	@echo "[COMPILE] compiling sandbox"
	@$(CC) -o $(PROGNAME) $^ $(LFLAGS)
	@echo "[RUN] Running sandbox"
	@./sandbox
	@echo "[CLEAN] Removing all compiled files"
	@rm -f *.o $(PROGNAME)

debug : CFLAGS += -DDEBUG -g
debug : all
	@echo "!! Built using debug flags !!"

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean :
	@echo "[CLEAN] Removing all sandbox files"
	@rm -f *.o $(PROGNAME)
	@rm -f output.bmp

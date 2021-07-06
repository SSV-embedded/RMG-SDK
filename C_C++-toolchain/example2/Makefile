CC=gcc
STRIP=strip
CFLAGS=
OPTS=-Wall
# -Werror

DEPS = log.c
PROJ1 = can_helloworld

all: clean $(PROJ1)

$(PROJ1): $(PROJ1).c $(DEPS) Makefile
	$(CC) $(CFLAGS) $(OPTS) $(DEPS) -o $@ $< -lmosquitto
	$(STRIP) $(PROJ1)

clean:
	rm -f $(PROJ1)

CC = gcc
CFLAGS = -Wall -Wextra -g

vpath %.c main:sound:wave:synthesis

OBJSDIR = build
LIBS = m asound
LIBSFULL = $(addprefix -l , $(LIBS))
OBJS = sound midi synthesis wave main conversion
OBJSFULL = 	$(addprefix $(OBJSDIR)/,\
		$(addsuffix .o, $(OBJS)))


gzatizer: $(OBJSFULL)
	$(CC) $(CFLAGS) $(LIBSFULL) -o $@ $^

$(OBJSDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJSDIR)/*

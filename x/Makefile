OBJS= main.o
APP= x378

LDFLAGS+= -lX11 -lXtst -lXext -l378 -L..
CFLAGS+= -DUSE_BZ2 -I..

all: $(APP)

$(APP): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -f $(APP) $(OBJS)

CC=$(EC535)/arm-linux/bin/arm-linux-gcc -static
PROGS = car_control
LDFLAGS = -lm

$(PROGS): car_control.o
	$(CC) car_control.o $(LDFLAGS) -o $(PROGS)

all: $(PROGS)


clean: 
	rm -f $(PROGS)  *.o *~

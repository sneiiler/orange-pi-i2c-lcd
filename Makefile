CC = gcc
LD = gcc

CFLAGS=-std=gnu99


all: oled_display_lidar_status

oled_display_lidar_status: oled_display_lidar_status.o oled.o
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf oled_display_lidar_status oled*.o

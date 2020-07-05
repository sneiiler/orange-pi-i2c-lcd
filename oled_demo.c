/*
 * Copyright (c) 2015, Vladimir Komendantskiy
 * MIT License
 *
 * SSD1306 demo of block and font drawing.
 */

//
// fixed for OrangePiZero by HypHop
//

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

#define IP_SIZE 16

#include "oled.h"
#include "font.h"

int get_local_ip(const char *eth_inf, char *ip); // get local IP
int is_network_up(char *chkhost, unsigned short chkport);

int oled_demo(struct display_info *disp)
{

	char ip[IP_SIZE];

	const char *test_eth = "eth0";

	get_local_ip(test_eth, ip);

	char *ip_head = "IP Address: ";

	char *notice = "-->";

	char *ip_message = (char *)malloc(strlen(notice) + strlen(ip));

	sprintf(ip_message, "%s%s", notice, ip);

	char *lidar_1_online_head = "Lidar 1 status: ";

	char *lidar_1_online_message = (char *)malloc(strlen(notice) + 1);

	sprintf(lidar_1_online_message, "%s%s", lidar_1_online_head, is_network_up("192.168.1.201", 22));

	char *lidar_2_online_head = "Lidar 2 status: ";

	char *lidar_2_online_message = (char *)malloc(strlen(notice) + 2);

	sprintf(lidar_2_online_message, "%s%s", lidar_2_online_head, is_network_up("192.168.1.201", 22));

	//putstrto(disp, 0, 0, "Spnd spd  2468 rpm");
	oled_putstrto(disp, 0, 0 + 0, ip_head);
	oled_putstrto(disp, 0, 9 + 1, ip_message);
	oled_putstrto(disp, 0, 18 + 2, lidar_1_online_message);
	oled_putstrto(disp, 0, 27 + 3, lidar_2_online_message);
	disp->font = font2;
	// oled_putstrto(disp, 0, 18 + 2, "Spnd tmp    53 C");
	// disp->font = font2;
	// oled_putstrto(disp, 0, 27 + 3, "DrvX tmp    64 C");
	// oled_putstrto(disp, 0, 36 + 4, "");
	// oled_putstrto(disp, 0, 45 + 5, "");
	// disp->font = font1;
	// oled_putstrto(disp, 0, 54, "Total cur  2.36 A");

	oled_putstrto(disp, 0, 54 + 4, "Designed by Enjoyer. All Right CopyRight.");

	oled_send_buffer(disp);

	// uint8_t buf[100];

	// disp->font = font3;
	// for (int i = 0; i < 100; i++)
	// {
	// 	sprintf(buf, "Spnd spd  %d rpm", i);
	// 	oled_putstrto(disp, 0, 0, buf);
	// 	oled_putstrto(disp, 135 - i, 36 + 4, "===");
	// 	oled_putstrto(disp, 100, 0 + i / 2, ".");
	// 	oled_send_buffer(disp);
	// }
	//oled_putpixel(disp, 60, 45);
	//oled_putstr(disp, 1, "hello");

	return 0;
}

void show_error(int err, int add)
{
	//const gchar* errmsg;
	//errmsg = g_strerror(errno);
	printf("\nERROR: %i, %i\n\n", err, add);
	//printf("\nERROR\n");
}

void show_usage(char *progname)
{
	printf("\nUsage:\n%s <I2C bus device node >\n", progname);
}

int get_local_ip(const char *eth_inf, char *ip)
{
	int sd;
	struct sockaddr_in sin;
	struct ifreq ifr;

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (-1 == sd)
	{
		printf("socket error: %s\n", strerror(errno));
		return -1;
	}

	strncpy(ifr.ifr_name, eth_inf, IFNAMSIZ);
	ifr.ifr_name[IFNAMSIZ - 1] = 0;

	// if error: No such device
	if (ioctl(sd, SIOCGIFADDR, &ifr) < 0)
	{
		printf("ioctl error: %s\n", strerror(errno));
		close(sd);
		return -1;
	}

	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
	snprintf(ip, IP_SIZE, "%s", inet_ntoa(sin.sin_addr));

	close(sd);
	return 0;
}

int is_network_up(char *chkhost, unsigned short chkport)
{
	int sock = -1;
	struct addrinfo *res, *rp;
	int ret = 0;
	char sport[10];
	snprintf(sport, sizeof sport, "%d", chkport);

	struct addrinfo hints = {.ai_socktype = SOCK_STREAM};

	if (getaddrinfo(chkhost, sport, &hints, &res))
	{
		perror("gai");
		return 0;
	}

	for (rp = res; rp && !ret; rp = rp->ai_next)
	{
		sock = socket(rp->ai_family, rp->ai_socktype,
					  rp->ai_protocol);
		if (sock == -1)
			continue;
		if (connect(sock, rp->ai_addr, rp->ai_addrlen) != -1)
		{
			char node[200], service[100];
			getnameinfo(res->ai_addr, res->ai_addrlen, node, sizeof node, service, sizeof service, NI_NUMERICHOST);

			// printf("Success on %s, %s\n", node, service);
			ret = 1; /* Success */
		}
		close(sock);
	}
	freeaddrinfo(res);

	return ret;
}

int main(int argc, char **argv)
{

	// ----------
	char filename[32];
	struct display_info disp;

	// if (argc < 2)
	// {
	// 	show_usage(argv[0]);
	// 	return -1;
	// }

	memset(&disp, 0, sizeof(disp));

	sprintf(filename, "%s", "/dev/i2s-0");
	disp.address = OLED_I2C_ADDR;
	disp.font = font2;

	//	if (oled_open(&disp, filename) < 0 ||
	//	    oled_init(&disp)           < 0 ||
	//	    oled_demo(&disp)           < 0)
	//	{

	int e;

	e = oled_open(&disp, filename);

	if (e < 0)
	{
		show_error(1, e);
	}
	else
	{
		e = oled_init(&disp);
		if (e < 0)
		{
			show_error(2, e);
		}
		else
		{

			printf("-----------------\n");

			if (oled_demo(&disp) < 0)
			{
				show_error(3, 777);
			}
		}
	}

	return 0;
}

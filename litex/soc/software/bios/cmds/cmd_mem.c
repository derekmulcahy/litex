// SPDX-License-Identifier: BSD-Source-Code

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <libbase/memtest.h>

#include <generated/csr.h>
#include <generated/mem.h>

#include "../command.h"
#include "../helpers.h"

/**
 * Command "mem_list"
 *
 * Memory list
 *
 */
static void mem_list_handler(int nb_params, char **params)
{
	printf("Available memory regions:\n");
	puts(MEM_REGIONS);
}

define_command(mem_list, mem_list_handler, "List available memory regions", MEM_CMDS);

/**
 * Command "mem_read"
 *
 * Memory read
 *
 */
static void mem_read_handler(int nb_params, char **params)
{
	char *c;
	unsigned int *addr;
	unsigned int length;

	if (nb_params < 1) {
		printf("mem_read <address> [length]");
		return;
	}
	addr = (unsigned int *)strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect address");
		return;
	}
	if (nb_params == 1) {
		length = 4;
	} else {
		length = strtoul(params[1], &c, 0);
		if(*c != 0) {
			printf("\nIncorrect length");
			return;
		}
	}

	dump_bytes(addr, length, (unsigned long)addr);
}

define_command(mem_read, mem_read_handler, "Read address space", MEM_CMDS);

/**
 * Command "mem_write"
 *
 * Memory write
 *
 */
static void mem_write_handler(int nb_params, char **params)
{
	char *c;
	void *addr;
	unsigned int value;
	unsigned int count;
	unsigned int size;
	unsigned int i;

	if (nb_params < 2) {
		printf("mem_write <address> <value> [count] [size]");
		return;
	}

	size = 4;
	addr = (void *)strtoul(params[0], &c, 0);

	if (*c != 0) {
		printf("Incorrect address");
		return;
	}

	value = strtoul(params[1], &c, 0);
	if(*c != 0) {
		printf("Incorrect value");
		return;
	}

	if (nb_params == 2) {
		count = 1;
	} else {
		count = strtoul(params[2], &c, 0);
		if(*c != 0) {
			printf("Incorrect count");
			return;
		}
	}

	if (nb_params == 4)
		size = strtoul(params[3], &c, 0);

	for (i = 0; i < count; i++) {
		switch (size) {
		case 1:
			*(uint8_t *)addr++ = value;
			break;
		case 2:
			*(uint16_t *)addr++ = value;
			break;
		case 4:
			*(uint32_t *)addr++ = value;
			break;
		default:
			printf("Incorrect size");
			return;
		}
	}
}

define_command(mem_write, mem_write_handler, "Write address space", MEM_CMDS);

/**
 * Command "mem_copy"
 *
 * Memory copy
 *
 */
static void mem_copy_handler(int nb_params, char **params)
{
	char *c;
	unsigned int *dstaddr;
	unsigned int *srcaddr;
	unsigned int count;
	unsigned int i;

	if (nb_params < 2) {
		printf("mem_copy <dst> <src> [count]");
		return;
	}

	dstaddr = (unsigned int *)strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect destination address");
		return;
	}

	srcaddr = (unsigned int *)strtoul(params[1], &c, 0);
	if (*c != 0) {
		printf("Incorrect source address");
		return;
	}

	if (nb_params == 2) {
		count = 1;
	} else {
		count = strtoul(params[2], &c, 0);
		if (*c != 0) {
			printf("Incorrect count");
			return;
		}
	}

	for (i = 0; i < count; i++)
		*dstaddr++ = *srcaddr++;
}

define_command(mem_copy, mem_copy_handler, "Copy address space", MEM_CMDS);

/**
 * Command "mem_test"
 *
 * Memory Test
 *
 */
static void mem_test_handler(int nb_params, char **params)
{
	char *c;
	unsigned int *addr;
	unsigned long maxsize = ~0uL;

	if (nb_params < 1) {
		printf("mem_test <addr> [<maxsize>]");
		return;
	}

	addr = (unsigned int *)strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect address");
		return;
	}

	if (nb_params >= 2) {
		maxsize = strtoul(params[1], &c, 0);
		if (*c != 0) {
			printf("Incorrect size");
			return;
		}

	}

	memtest(addr, maxsize);
}
define_command(mem_test, mem_test_handler, "Test memory access", MEM_CMDS);

/**
 * Command "mem_speed"
 *
 * Memory Speed
 *
 */
static void mem_speed_handler(int nb_params, char **params)
{
	char *c;
	unsigned int *addr;
	unsigned long size;
	bool read_only = false;
	bool random = false;

	if (nb_params < 1) {
		printf("mem_speed <addr> <size> [<readonly>] [<random>]");
		return;
	}

	addr = (unsigned int *)strtoul(params[0], &c, 0);
	if (*c != 0) {
		printf("Incorrect address");
		return;
	}

	size = strtoul(params[1], &c, 0);
	if (*c != 0) {
		printf("Incorrect size");
		return;
	}

	if (nb_params >= 3) {
		read_only = (bool) strtoul(params[2], &c, 0);
		if (*c != 0) {
			printf("Incorrect readonly value");
			return;
		}
	}

	if (nb_params >= 4) {
		random = (bool) strtoul(params[3], &c, 0);
		if (*c != 0) {
			printf("Incorrect random value");
			return;
		}
	}

	memspeed(addr, size, read_only, random);
}
define_command(mem_speed, mem_speed_handler, "Test memory speed", MEM_CMDS);

#ifndef __CONVERSION_H
#define __CONVERSION_H

/*Little endian to big endian*/
unsigned ltobendian(unsigned char *lendian, int valsize);

/* Int to raw big endian*/
unsigned char *tobendian(unsigned char *dest, int value, int valsize);

/*Little endian to int*/
int *letoint(int *dest, unsigned char *lendian, int valsize, int totalsize);

/*Raw signed int bytes to int*/
int tosint(unsigned bendian, int valsize);

#endif

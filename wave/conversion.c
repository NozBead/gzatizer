/*Little endian to big endian*/
unsigned ltobendian(unsigned char *lendian, int valsize) {
	unsigned value = 0;
	for (valsize-- ; valsize >= 0 ; valsize--) {
		value |= lendian[valsize] << valsize * 8;
	}
	return value;
}

/* Int to raw big endian*/
unsigned char *tobendian(unsigned char *dest, int value, int valsize) {
	for (int i = 0 ; i < valsize ; i++) {
		dest[i] = value >> ((valsize - 1 - i) * 8) & 0xFF;
	}
	return dest;
}

/*Raw signed int bytes to int*/
int tosint(unsigned bendian, int valsize) {
	unsigned signmsk = 1 << (valsize * 8 - 1);
	int val = bendian & ~signmsk;
	val -= bendian & signmsk;
	return val;
}

/*Little endian to int*/
int *letoint(int *dest, unsigned char *lendian, int valsize, int totalsize) {
	for (int i = 0 ; i < totalsize ; i++) {
		unsigned bendian = ltobendian(lendian + (i * valsize), valsize);
		dest[i] = tosint(bendian, valsize);
	}
	return dest;
}

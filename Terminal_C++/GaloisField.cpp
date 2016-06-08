#include "GaloisField.h"

void GaloisField::precomputeInput()
{
	short i;
	for (i = 1; i < m; i++)
		p[i] = 0;
	p[0] = p[m] = 1;
	p[1] = 1;
	n = 1;
	for (i = 0; i <= m; i++) {
		n *= 2;
	}
	n = n / 2 - 1;

}

void  GaloisField::getGaloisField()
{
	int i, mask;
	mask = 1;
	alpha_to[m] = 0;
	mask = 1;
	alpha_to[m] = 0;
	for (i = 0; i < m; i++) {
		alpha_to[i] = mask;
		index_of[alpha_to[i]] = i;
		if (p[i] != 0)
			alpha_to[m] ^= mask;
		mask <<= 1;
	}
	index_of[alpha_to[m]] = m;
	mask >>= 1;
	for (i = m + 1; i < n; i++) {
		if (alpha_to[i - 1] >= mask)
			alpha_to[i] = alpha_to[m] ^ ((alpha_to[i - 1] ^ mask) << 1);
		else
			alpha_to[i] = alpha_to[i - 1] << 1;
		index_of[alpha_to[i]] = i;
	}
	index_of[0] = -1;
}

short * GaloisField::generatePolinom()
{
	short	ii, jj, ll, kaux;
	short	test, aux, nocycles, root, noterms;
	short   cycle[512][10], size[1024], min[1024], zeros[1024];
	short * g = (short*)malloc(512*sizeof(short));
	cycle[0][0] = 0;
	size[0] = 1;
	cycle[1][0] = 1;
	size[1] = 1;
	jj = 1;

	if (m > 9) 
	{
#ifdef LOG
		printf("Computing cycle sets modulo %d\n", n);
		printf("(This may take some time)...\n");
#endif
	}
	do {
		ii = 0;
		do {
			ii++;
			cycle[jj][ii] = (cycle[jj][ii - 1] * 2) % n;
			size[jj]++;
			aux = (cycle[jj][ii] * 2) % n;
		} while (aux != cycle[jj][0]);

		ll = 0;
		do {
			ll++;
			test = 0;
			for (ii = 1; ((ii <= jj) && (!test)); ii++)
				for (kaux = 0; ((kaux < size[ii]) && (!test)); kaux++)
					if (ll == cycle[ii][kaux])
						test = 1;
		} while ((test) && (ll < (n - 1)));
		if (!(test)) {
			jj++;
			cycle[jj][0] = ll;
			size[jj] = 1;
		}
	} while (ll < (n - 1));
	nocycles = jj;
	kaux = 0;
	rdncy = 0;
	for (ii = 1; ii <= nocycles; ii++) {
		min[kaux] = 0;
		test = 0;
		for (jj = 0; ((jj < size[ii]) && (!test)); jj++)
			for (root = 1; ((root < d) && (!test)); root++)
				if (root == cycle[ii][jj]) {
					test = 1;
					min[kaux] = ii;
				}
		if (min[kaux]) {
			rdncy += size[min[kaux]];
			kaux++;
		}
	}
	noterms = kaux;
	kaux = 1;
	for (ii = 0; ii < noterms; ii++)
		for (jj = 0; jj < size[min[ii]]; jj++) {
			zeros[kaux] = cycle[min[ii]][jj];
			kaux++;
		}

	k = length - rdncy;

	if (k<0)
	{
		printf("Parameters invalid!\n");
		exit(0);
	}
#ifdef  LOG
	printf("This is a (%d, %d, %d) binary BCH code\n", length, k, d);
#endif //  DEBUG
	g[0] = alpha_to[zeros[1]];
	g[1] = 1;
	for (ii = 2; ii <= rdncy; ii++) {
		g[ii] = 1;
		for (jj = ii - 1; jj > 0; jj--)
			if (g[jj] != 0)
				g[jj] = g[jj - 1] ^ alpha_to[(index_of[g[jj]] + zeros[ii]) % n];
			else
				g[jj] = g[jj - 1];
		g[0] = alpha_to[(index_of[g[0]] + zeros[ii]) % n];
	}
#ifdef LOG
	FILE* f = fopen("default.txt", "a+");
	fprintf(f, "public final Galois[] = {", g[ii]);
	for (ii = 0; ii <= rdncy; ii++) {
		printf("%d:%d ", ii,g[ii]);
		fprintf(f, "(byte)%d,", g[ii]);
		if (ii && ((ii % 50) == 0))
			fprintf(f,"%s","\n");
	}
	fprintf(f, "%s","}");
#endif
	return g;
}

int GaloisField::getLastRedundancy()
{
	return rdncy;
}

GaloisField::GaloisField(short m,short length,short t)
{
	this->m = m;
	this->length = length;
	this->t = t;
	d = 2 * t + 1;
	memset(alpha_to, 0x00, 1024);
	memset(index_of, 0x00, 1024);
	precomputeInput();
	getGaloisField();
}

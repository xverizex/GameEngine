#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

static float *f; /* sizes vertex */
static int f_size; /* faces size */

static float *v;
static int nv;

static float *vn;
static int nvn;

static float *vt;
static int nvt;

static void v_parser (char *l) { 
	char *s = strstr (l, " ");
	s++;

	l = s;
	char p[32];
	int pn = 0;

	int index = nv++;
	v = realloc (v, sizeof (float) * nv * 3);

	for (int i = 0; i < 3; i++) {
		while (*l != ' ' && *l != '\n') {
			p[pn++] = *l++;
		}
		l++;

		p[pn] = 0;

		v[index * 3 + i] = atof (p);

		pn = 0;
	}
}
static void vn_parser (char *l) {
	char *s = strstr (l, " ");
	s++;

	l = s;
	char p[32];
	int pn = 0;

	int index = nvn++;
	vn = realloc (vn, sizeof (float) * nvn * 3);

	for (int i = 0; i < 3; i++) {
		while (*l != ' ' && *l != '\n') {
			p[pn++] = *l++;
		}
		p[pn] = 0;
		l++;

		vn[index * 3 + i] = atof (p);

		pn = 0;
	}
}
static void vt_parser (char *l) {
	char *s = strstr (l, " ");
	s++;

	l = s;
	char p[32];
	int pn = 0;

	int index = nvt++;
	vt = realloc (vt, sizeof (float) * nvt * 2);

	for (int i = 0; i < 2; i++) {
		while (*l != ' ' && *l != '\n') {
			p[pn++] = *l++;
		}
		p[pn] = 0;
		l++;

		vt[index * 2 + i] = atof (p);

		pn = 0;
	}

}
static void f_parser (char *l) {
	char *s = strstr (l, " ");
	s++;

	l = s;
	int index = f_size++;
	f = realloc (f, sizeof (float) * f_size * 24);

	char p[32];
	int pn = 0;
	int is = 0;

	float *m = NULL;
	int nn = 0;

	for (int i = 0; i < 23; i += nn) {
		while (*l != '/' && *l != ' ' && *l != '\n') {
			p[pn++] = *l++;
		}
		p[pn] = 0;
		l++;

		pn = 0;

		if (i == 0 || i == 8 || i == 16) { m = v; nn = 3; }
		if (i == 3 || i == 11 || i == 19) { m = vt; nn = 2; }
		if (i == 5 || i == 13 || i == 21) { m = vn; nn = 3; }

		int pos = atoi (p) - 1;

		memcpy (&f[index * 24 + is], &m[pos * nn], sizeof (float) * nn);
		is += nn;
		if (i == 22) break;
	}
}

struct tokens {
	char *name;
	int size;
	void (*func) (char *line);
} t[] = {
	{ "v ", 2, v_parser },
	{ "vn ", 3, vn_parser },
	{ "vt ", 3, vt_parser },
	{ "f ", 2, f_parser }
};

static char *get_name (char *arg)
{
	char *s = strrchr (arg, '.');
	if (s) { *s = 0; }

	char *name = calloc (255, 1);
	snprintf (name, 255, "%s.object", arg);

	return name;
}

int main (int argc, char **argv)
{
	if (argc < 2) { printf ("error: no select file\n"); return -1; }

	FILE *fp = fopen (argv[1], "r");

	int size_tokens = sizeof (t) / sizeof (struct tokens);

	char line[512];

	while (fgets (line, 512, fp)) {
		for (int i = 0; i < size_tokens; i++) {
			int found = 1;
			for (int s = 0; s < t[i].size; s++) {
				if (t[i].name[s] != line[s]) {
					found = 0;
					break;
				}
			}
			if (found) t[i].func (line);
		}
	}

	fclose (fp);

	char *new_name = get_name (argv[1]);

	int i = 1;
	fp = fopen (new_name, "w");
	fwrite (&i, sizeof (int), 1, fp);
	fwrite (&i, sizeof (int), 1, fp);
	fwrite (&f_size, sizeof (int), 1, fp);
	fwrite (f, sizeof (float) * 24, f_size, fp);

	fclose (fp);
}

/*
 *
 *
 */

#define _GNU_SOURCE  /* strcasestr() */
#include <string.h>
#include <dirent.h>
#include <fnmatch.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


/* computes a simple (fast) checksum on string s */
static uint32_t csum(const char *s) {
  uint32_t r = 1985;
  while (*s != 0) {
    uint32_t msb;
    msb = r >> 31; /* rol 1 */
    r <<= 1;       /* rol 1 */
    r |= msb;      /* rol 1 */
    r ^= *s;       /* xor *s */
    s++;
  }
  return(r);
}


#ifdef HEXSTUFF
/* encode a string into hex */
static void str2hex(char *d, const char *s) {
  const char alphabet[16] = "0123456789abcdef";
  int i, di = 0;
  for (i = 0; s[i] != 0; i++) {
    d[di++] = alphabet[(s[i] >> 4)];
    d[di++] = alphabet[s[i] & 0xff];
  }
  d[di] = 0;
}

/* translates a hex char (0..f) into a char val (0..15) */
static char hchar2val(char h) {
  if ((h >= '0') && (h <= '9')) return(h - '0');
  if ((h >= 'a') && (h <= 'f')) return(h - 'a' + 10);
  return(h - 'A' + 10);
}


static void hex2str(char *d, const char *s) {
  int i, di = 0;
  for (i = 0; s[i] != 0; i += 2) {
    if (s[i+1] == 0) break;
    /* read 2 bytes */
    d[di] = hchar2val(s[i] << 4);
    d[di++] |= hchar2val(s[i+1]);
  }
  d[di] = 0;
}
#endif


/* used by getartlist() for getting a list of txt files.
 * returns 1 if d is a txt file, zero otherwise. */
static int filtertxt(const struct dirent *d) {
  /* must not be a directory */
  if (d->d_type == DT_DIR) return(0);
  /* must have a txt extension */
  if (fnmatch("*.txt", d->d_name, 0) != 0) return(0);
  /* success */
  return(1);
}


/* returns 0 for filenames that begin with a dot, 1 otherwise */
static int filternodot(const struct dirent *d) {
  if (d->d_name[0] == '.') return(0);
  return(1);
}


static int getartlist(struct dirent ***r) {
  return(scandir(".", r, filtertxt, alphasort));
}


/*static int strstartswith(const char *str, const char *prefix) {
  for (;;) {
    if (*prefix == 0) return(1)
    if (*str != *prefix) return(0);
    prefix++;
    str++;
  }
}*/


/* returns the title of an article (ie. 1st line of the file) */
static char *get_artfile_title(const char *fname, char *buff, size_t bufflen) {
  FILE *fd;
  char *r;
  int i;

  fd = fopen(fname, "rb");
  if (fd == NULL) return("ERROR");
  r = fgets(buff, bufflen, fd);
  fclose(fd);
  if (r == NULL) return("ERROR");
  /* skip 'title:' prefix, if set */
  if (strcasestr(r, "Title:") == r) r += 6;
  /* skip any leading spaces */
  while (*r == ' ') r++;
  /* trim at first \r or \n */
  for (i = 0; (r[i] != '\r') && (r[i] != '\n') && (r[i] != 0); i++);
  r[i] = 0;
  return(r);
}


/* displays the list of articles */
static void display_artlist(void) {
  struct dirent **list = NULL;
  int listlen;
  char buff[512];
  listlen = getartlist(&list);
  /* display articles *in reverse order* */
  while (--listlen >= 0) {
    printf("1%s\t?a%x\n", get_artfile_title(list[listlen]->d_name, buff, sizeof(buff)), csum(list[listlen]->d_name));
  }
  free(list);
}


static void dump_attachmentslist(const char *fname) {
  struct dirent **list = NULL;
  int listlen;
  int i;
  char dirname[256];

  snprintf(dirname, sizeof(dirname), "%s", fname);

  /* trim fname to drop the file extension (.txt) */
  for (i = strlen(dirname) - 1; i >= 0; i--) {
    if (dirname[i] == '.') {
      dirname[i] = 0;
      break;
    }
  }

  listlen = scandir(dirname, &list, filternodot, alphasort);
  if (listlen > 0) printf("i=== Attachments ==========================================\ni\n");

  for (i = 0; i < listlen; i++) {
    printf("9%s\t%s/%s\n", list[i]->d_name, dirname, list[i]->d_name);
  }

  free(list);
}


static void dump_artfile(const char *fname) {
  char linebuf[1024];
  FILE *fd;
  int bodyflag = 0;

  /* open file */
  fd = fopen(fname, "rb");
  if (fd == NULL) {
    printf("3failed to open file");
    return;
  }

  /* dump lines, but first skip all headers */
  while (fgets(linebuf, sizeof(linebuf), fd) != NULL) {
    if (bodyflag != 0) printf("i%s", linebuf);
    if ((linebuf[0] == '\r') || (linebuf[0] == '\n')) bodyflag = 1;
  }

  fclose(fd);

  printf("i\n");

  /* print out attachments, if any */
  dump_attachmentslist(fname);
}


static void display_art(uint32_t artid) {
  struct dirent **list = NULL;
  int listlen;
  char titbuf[256];

  /* look for an article file that matches artid */
  listlen = getartlist(&list);
  while (--listlen >= 0) {
    if (csum(list[listlen]->d_name) == artid) break;
  }

  if (listlen < 0) {
    printf("3article not found\n");
    printf("1Back to main screen\n");
    return;
  }

  /* print title */
  printf("i\ni\ni### %s ###\ni\ni\n", get_artfile_title(list[listlen]->d_name, titbuf, sizeof(titbuf)));

  dump_artfile(list[listlen]->d_name);

  free(list);
}


int main(void) {
  const char *query;
  char action = 0;
  uint32_t param = 0;

  query = getenv("QUERY_STRING");
  if (query != NULL) {
    action = query[0];
    if (query[0] != 0) param = (uint32_t)(strtoul(query + 1, NULL, 16));
  }

  /* is this a request for an article? */
  if (action == 'a') {
    display_art(param);
    return(0);
  }

  /* display intro text */
  dump_artfile("main");
  printf("i\n");

  /* display the list of all articles */
  display_artlist();

  return(0);
}

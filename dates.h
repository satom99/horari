extern char *nomdiaset[];

/* Canvi de data de calendari a data juliana*/
double c2jd (double y, double m, double d);

/* Canvi de data juliana a data de calendari */
void jd2c (double jd, double *yy, double *mm, double *dd);

/* Suposant que estem a l'any acadèmic curs-(curs+1), torna el dia de la
 * setmana de la data dd/mm */
int mm2any (int any, int mm);

/* Suposant que estem a l'any acadèmic curs-(curs+1), incrementa la data
 * (*dd)/(*mm) en ndies dies. */
void incrdata (int any, int *dd, int *mm, int ndies);

/* Suposant que estem a l'any acadèmic curs-(curs+1), torna el dia de la
 * setmana de la data dd/mm */
int data2diaset (int any, int dd, int mm);

/* Donat l'string s, torna:
 * - 0 si és "dl",
 * - 1 si és "dm",
 * - ...
 * - 6 si és "dg",
 * - 7 si no és cap dels anteriors. */
int str2diaset (char *s);

#include <stdio.h>
#include <string.h>
#include <math.h>

char *nomdiaset[7] = { "dl", "dm", "dc", "dj", "dv", "ds", "dg" };

/*
 * Canvi de data de calendari a data juliana
 */
double c2jd (double y, double m, double d) {
   double a, b;
   if (m<=2) { y--; m+=12; }
   a=floor(y/100);
   b=y+0.01*m+0.0001*d>=1582.1015 ?
      2-a+floor(a/4)	/* correcció per la reforma gregoriana */
      : 0;
   return floor(365.25*(y+4716))+floor(30.6001*(m+1))+d+b-1524.5;
}

/*
 * Canvi de data juliana a data de calendari
 */
void jd2c (double jd, double *yy, double *mm, double *dd) {
   double z, f, b, c, d, e;
   jd+=0.5;
   z=floor(jd);
   f=jd-z;
   b=z;
   if (z>=2299161) {	/* correcció per la reforma gregoriana */
      double alf=floor((z-1867126.25)/36524.25);
      b+=1+alf-floor(alf/4);
   }
   b+=1524;
   c=floor((b-122.1)/365.25);
   d=floor(365.25*c);
   e=floor((b-d)/30.6001);
   *dd=b-d-floor(30.6001*e)+f;
   *mm=e<14 ? e-1 : e-13;
   *yy=4716;
   if (*mm<=2) (*yy)--;
   *yy=c-(*yy);
}

/*
 * Suposant que som a l'any acadèmic curs-(curs+1), donat un mes
 * torna l'any corresponent
 */
int mm2any (int curs, int mm) {
  if (mm < 9)
    return curs + 1;
  else
    return curs;
}

/*
 * Suposant que estem a l'any acadèmic curs-(curs+1), incrementa la data
 * (*dd)/(*mm) en ndies dies.
 */
void incrdata (int curs, int *dd, int *mm, int ndies) {
  double year = mm2any(curs, *mm);
  double juli = c2jd(year, *mm, *dd);
  juli += ndies;

  double month = *mm,
         day = *dd;

  jd2c(juli, &year, &month, &day);

  *mm = month;
  *dd = day;
}

/*
 * Suposant que estem a l'any acadèmic curs-(curs+1), torna el dia de la
 * setmana de la data dd/mm
 */
/* Juli -> Data des d'un dilluns */
int data2diaset (int curs, int dd, int mm) {
  double year = mm2any(curs, mm);
  double juli = c2jd(year, mm, dd);
  double real = juli + .5;

  return fmod(real, 7);
}

/*
 * Donat l'string s, torna:
 * - 0 si és "dl",
 * - 1 si és "dm",
 * - ...
 * - 6 si és "dg",
 * - 7 si no és cap dels anteriors.
 */
int str2diaset (char *s) {
  int i;
  for (i = 0; i < 7; i++) {
    if (strcmp(nomdiaset[i], s) == 0) {
      break;
    }
  }
  return i;
}

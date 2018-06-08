#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dates.h"

int read_chunk(double** chunk);
void read_term(char* term, int* halted);
int read_assig(char* term, char* assig);
int is_nolectiu(int year, int month, int day, int nolecount, double* nolectius);
void print_spec(int year, int month, int day, int hour_0, int hour_1, char* assig, char* tipus, char* group, char* class);

int curs = 0;

int main(int argc, char **argv) {
  // Required params
  char* arg = argv[argc - 1];

  sscanf(arg, "%d", &curs);

  if (!curs) {
    printf("Error: és necessari introduïr el curs acadèmic.\n");
    return 1;
  }

  // Variables
  double* weeks;
  double* nolectius;
  int nolecount;

  // Setmanes
  read_chunk(&weeks);

  // Dies no lectius
  nolecount = \
  read_chunk(&nolectius);

  // Parsing
  /*
    States:
    - 0: assig
    - 1: spec
    - 2: days
  */
  int state = 0,
      halted = 0;

  char term[64];

  char assig[13],
       tipus[13],
       group[13],
       wkday[13],
       class[13];

  int hour_0, hour_1;

  read_term(term, &halted);

  while(!halted) {
    //printf("%d - %s\n", state, term);
    switch(state) {
      case 0:
        read_assig(term, assig);

        read_term(term, &halted);
        state = 1;
        //
      case 1:
        if (read_assig(term, assig)) {
          state = 0;
          break;
        }
        strcpy(tipus, term);
        scanf(" grup%s %s %d-%d aula%s", group, wkday, &hour_0, &hour_1, class);

        read_term(term, &halted);
        state = 2;
        //
      case 2:
        assert(1);
        // Variables
        int week_0 = 0,
            week_1 = 0,
            month = 0,
            day = 0;

        sscanf(term, "setm%d-%d", &week_0, &week_1) || \
        sscanf(term, "setm%d", &week_0) || \
        sscanf(term, "%d/%d", &day, &month);

        // Are we done yet?
        if (!week_0 && !day) {
          state = 1;
          break;
        }

        int offset = str2diaset(wkday);

        if (week_0) {
          int week_count = week_1 - week_0;
          if (week_count < 1) {
            week_count = 0;
          }

          int i;
          for(i = 0; i <= week_count; i++) {
            int index = week_0 + i - 1;
            double week = weeks[index];
            double juli = week + offset;

            double yy, mm, dd;
            jd2c(juli, &yy, &mm, &dd);

            int year = yy;
            month = mm;
            day = dd;

            // Lectius
            if(is_nolectiu(year, month, day, nolecount, nolectius))
              continue;

            print_spec(
              year, month, day,
              hour_0, hour_1,
              assig, tipus, group, class
            );
          }
        } else {
          int year = mm2any(curs, month);

          // Lectius
          if(is_nolectiu(year, month, day, nolecount, nolectius)) {
            printf("\nError: el dia %02d/%02d és festiu. (%s)\n", month, day, assig);
            return 2;
          }

          // Week day
          if(data2diaset(curs, day, month) != offset) {
            printf("\nError: el dia %02d/%02d no és %s. (%s)\n", month, day, wkday, assig);
            return 3;
          }

          print_spec(
            year, month, day,
            hour_0, hour_1,
            assig, tipus, group, class
          );
        }

        read_term(term, &halted);
    }
  }

  // Cleanup
  free(weeks);
  free(nolectius);

  return 0;
}

int read_chunk(double** chunk) {
  // Count
  int count = 0;
  scanf(" %d", &count);

  assert(count);

  // Memory
  *chunk = calloc(count, sizeof(double));

  int i;
  for (i = 0; i < count; i++) {
    double day;
    double month;

    scanf(" %lf/%lf ", &day, &month);

    double year = mm2any(curs, month);
    double juli = c2jd(year, month, day);

    (*chunk)[i] = juli;
  }

  return count;
}

void read_term(char* term, int* halted) {
  *halted = scanf("%s", term) != 1;
}

int read_assig(char* term, char* assig) {
  return sscanf(term, "assig%s", assig);
}

int is_nolectiu(int year, int month, int day, int count, double* nolectius) {
  double mm = month, dd = day;
  double check = c2jd(year, mm, dd);

  int i;
  for(i = 0; i < count; i++) {
    if (nolectius[i] == check) {
      break;
    }
  }

  return i != count;
}

void print_spec(int year, int month, int day, int hour_0, int hour_1, char* assig, char* tipus, char* group, char* class) {
  int hour_count = hour_1 - hour_0;

  int i;
  for(i = 0; i < hour_count; i++) {
    printf(
     "%04d%02d%02d%02d %s %s %s %s\n",
     year, month, day, hour_0 + i,
     assig, tipus, group, class
    );
  }
}

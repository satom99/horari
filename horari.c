#include <stdio.h>
#include <string.h>
#include "dates.h"

int read_spec(int* year, int* month, int* day, int* hour, char* assig, char* tipus, char* group, char* class);
void print_blank();
void print_spacer(char divider);

double week = 0;

int main(int argc, char **argv) {
  // Required params
  int curs = 0,
      month = 0,
      day = 0;

  if (argc == 3) {
    char* one = argv[argc - 2];
    char* two = argv[argc - 1];

    sscanf(one, "%d", &curs);
    sscanf(two, "%d/%d", &day, &month);
  }

  if (!curs || !month || !day) {
    printf("Error: és necessari introduïr el curs acadèmic i un dia.\n");
    return 1;
  }

  // Week
  double yy = mm2any(curs, month),
         mm = month,
         dd = day;

  week = c2jd(yy, mm, dd);

  // Parse
  char days[5][13][3][26] = {0};

  int year, hour;
  char assig[13],
       tipus[13],
       group[13],
       class[13];

  while(read_spec(&year, &month, &day, &hour, assig, tipus, group, class)) {
    double juli = c2jd(year, month, day);
    double diff = juli - week;
    int diaset = diff;

    if (diff < 0 || diff > 4)
      continue;

    int hora = hour - 8;

    char thing[26];
    sprintf(thing, "%s-%s", tipus, group);

    strcpy(days[diaset][hora][0], assig);
    strcpy(days[diaset][hora][1], thing);
    strcpy(days[diaset][hora][2], class);
  }

  ////////////
  // Output //
  ////////////
  char hours[13][6] = {
    "08-09",
    "09-10",
    "10-11",
    "11-12",
    "12-13",
    "13-14",
    "14-15",
    "15-16",
    "16-17",
    "17-18",
    "18-19",
    "19-20",
    "20-21"
  };

  ////////////////
  // First line //
  ////////////////
  print_blank();
  printf("  ");

  int i;
  for(i = 0; i < 5; i++) {
    char* nom = nomdiaset[i];
    double juli = week + i;

    double yy, mm, dd;
    jd2c(juli, &yy, &mm, &dd);

    int month = mm, day = dd;
    printf("%s %02d/%02d", nom, day, month);
    print_blank();
  }
  putchar('\n');

  print_spacer('.');

  ////////////
  // Horari //
  ///////////
  int row;
  for(row = 0; row < 52; row++) {
    int item = row % 4;
    int index = row / 4;

    // Spacers
    if (item == 3) {
      print_spacer('|');
      continue;
    }

    // Left-side
    char text[7] = {0};
    if (item == 0) {
      strcpy(text, hours[index]);
    }
    printf("%-6s|", text);

    // Blocks
    int column;
    for(column = 0; column < 5; column++) {
      char text[13] = {0};

      strcpy(text, days[column][index][item]);

      printf(" %-12s|", text);
    }
    putchar('\n');
  }
  putchar('\f');

  return 0;
}

int read_spec(int* year, int* month, int* day, int* hour, char* assig, char* tipus, char* group, char* class) {
  return scanf("%04d%02d%02d%02d %s %s %s %s", year, month, day, hour, assig, tipus, group, class) == 8;
}

void print_blank() {
  printf("%-6s", "");
}

void print_spacer(char divider) {
  print_blank();

  int count = 14*5 + 1;

  int i;
  for(i = 0; i < count; i++) {
    if (i % 14 == 0) {
      putchar(divider);
    } else {
      putchar('.');
    }
  }
  putchar('\n');
}

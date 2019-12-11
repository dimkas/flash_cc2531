/***********************************************************************
  Copyright © 2019 Jean Michault.
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

//#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <gpiod.h>
//#include "CCDebugger.h"

void helpo()
{
  fprintf(stderr,"usage : cc_chipid <chip name> [-d pin_DD] [-c pin_DC] [-r pin_reset]\n"); 
  fprintf(stderr,"	-c : change pin_DC (default 27)\n");
  fprintf(stderr,"	-d : change pin_DD (default 28)\n");
  fprintf(stderr,"	-r : change reset pin (default 24)\n");
}

int main(int argc,char *argv[])
{
  
  int opt;
  int rePin=-1;
  int dcPin=-1;
  int ddPin=-1;
 
  char *name;

  while( (opt=getopt(argc,argv,"d:c:r:h?")) != -1)
  {
    switch(opt)
    {
     case 'd' : // DD pinglo
      ddPin=atoi(optarg);
      break;
     case 'c' : // DC pinglo
      dcPin=atoi(optarg);
      break;
     case 'r' : // restarigi pinglo
      rePin=atoi(optarg);
      break;
     case 'h' : // helpo
     case '?' : // helpo
      helpo();
      exit(0);
      break;
    }
  }
  name = argv[1];
  struct gpiod_chip *chip;
  chip = gpiod_chip_open_by_name(name);
  if (chip) {
      printf("Chip %s/%s open\n", 
             gpiod_chip_name(chip), 
             gpiod_chip_label(chip));
  }
  else {
      printf("Chip %s not find\n", name);
      exit(-1);
  }

  struct gpiod_line *test_line = gpiod_chip_get_line(chip,151);
  
  if (test_line) {
      printf("Line number %d config direction %d mode %d\n",
        gpiod_line_offset(test_line),
        gpiod_line_direction(test_line),
        gpiod_line_active_state(test_line)
    );
  }
  else {
      printf("Can't get line\n");
  }
  
  char *consumer = "cc-debugger";
    
  if (test_line) {
    int test_out = gpiod_line_request_output(
        test_line, consumer, 0);
    printf("Got output switch result %d\n", test_out);
    int test_value = gpiod_line_get_value(test_line);
    printf("Got value %d\n", test_value);
    sleep(20);
    test_out = gpiod_line_set_value(test_line, 1);
    printf("Set value result %d\n", test_out);
    sleep(3);
    test_value = gpiod_line_get_value(test_line);
    printf("Got value %d\n", test_value);
    sleep(3);
    gpiod_line_release(test_line);
    printf("Line released\n");
    sleep(30);
  }
  
  gpiod_chip_close(chip);
  // initialize GPIO and debugger
  //cc_init(chip, rePin, dcPin, ddPin);
  // enter debug mode
  //cc_enter();
  // get ChipID :
  //uint16_t res;
  //res = cc_getChipID();
  //printf("  ID = %04x.\n",res);
  //cc_setActive(false);
}

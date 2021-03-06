#include <minix/drivers.h>

#include "timer.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {
  sef_startup();
  if ( argc == 1 ) {
      print_usage(argv);
      return 0;
  } else {
      proc_args(argc, argv);
  }
  return 0;
}

static void print_usage(char *argv[]) {
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"packet <decimal number>\" \n"
			"\t service run %s -args \"async <decimal number>\" \n"
			"\t service run %s -args \"config \" \n"
			"\t service run %s -args \"gesture <decimal number>\" \n",
			argv[0], argv[0], argv[0],argv[0]);
}

static int proc_args(int argc, char *argv[]) {

  unsigned short cnt,idle_time,tolerance;
  short length;

 if (strncmp(argv[1], "packet", strlen("packet")) == 0) {
	  if( argc != 3 ) {
		  printf("packet: wrong no, of arguments for mouse_test_packet() \n");
		  return 1;
	  }
	  if( (cnt = parse_ulong(argv[2], 10)) == ULONG_MAX )
		  return 1;
	  printf("mouse::test_packet(%d) \n", cnt);
	  if (test_packet(cnt)){
		  printf("Mouse_test_packet function failed! \n");
		  return 1;
	  }
	  return 0;
  } else if (strncmp(argv[1], "async", strlen("async")) == 0) {
	  if( argc != 3 ) {
		  printf("async: wrong no. of arguments for mouse_test_async() \n");
		  return 1;
	  }
	  if( (idle_time = parse_ulong(argv[2], 10)) == ULONG_MAX )
		  return 1;
	  printf("mouse::mouse_test_async(%d)\n", idle_time);
	  if (test_async (idle_time))
	  {
		  printf("Mouse_test_async function failed! \n ");
		  return 1;
	  }
	  return 0;
  } else if (strncmp(argv[1], "config", strlen("config")) == 0) {
	  if( argc != 2 ) {
		  printf("config: wrong no. of arguments for mouse_test_config() \n");
		  return 1;
	  }
	  printf("mouse::mouse_test_config()\n");
	  if (test_config())
	  {
		  printf("Mouse_test_config function failed\n");
		  return 1;
	  }
	  return 0;
  }else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
	  if( argc != 3 ) {
		  printf("gesture: wrong no. of arguments for mouse_test_gesture() \n");
		  return 1;
	  }
	  if( (length = parse_ulong(argv[2], 10)) == ULONG_MAX )
			  return 1;

	  printf("mouse::mouse_test_gesture(%d)\n",length);
	  if (test_gesture(length))
	  {
		  printf("Mouse_test_gesture function failed\n");
		  return 1;
	  }
	  return 0;
  } else {
	  printf("Function invalid \"%s\" to test\n", argv[1]);
	  return 1;
  }
}

static unsigned long parse_ulong(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtoul(str, &endptr, base);

  if ((errno == ERANGE && val == ULONG_MAX )
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return ULONG_MAX;
  }

  if (endptr == str) {
	  printf("mouse: parse_ulong: no digits were found in %s \n", str);
	  return ULONG_MAX;
  }

  return val;
}

static long parse_long(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtol(str, &endptr, base);

  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return LONG_MAX;
  }

  if (endptr == str) {
	  printf("mouse: parse_long: no digits were found in %s \n", str);
	  return LONG_MAX;
  }

  return val;
}
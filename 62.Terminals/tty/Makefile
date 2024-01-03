all: new_intr no_echo test_partial_read test_tty_functions

new_intr: error_functions.c new_intr.c
	gcc -o new_intr error_functions.c new_intr.c

no_echo: error_functions.c no_echo.c
	gcc -o no_echo error_functions.c no_echo.c

test_partial_read: test_partial_read.c
	gcc -o test_partial_read test_partial_read.c

test_tty_functions: test_tty_functions.c
	gcc -o test_tty_functions error_functions.c tty_functions.c test_tty_functions.c

demo_SIGWINCH: demo_SIGWINCH.c
	gcc -o demo_SIGWINCH error_functions.c demo_SIGWINCH.c

exercises62-1: exercises62-1.c
	gcc -o exercises62-1 exercises62-1.c

exercises62-2: exercises62-2.c
	gcc -o exercises62-2 exercises62-2.c

exercises62-3: exercises62-3.c
	gcc -o exercises62-3 exercises62-3.c

exercises62-4: exercises62-4.c
	gcc -o exercises62-4 exercises62-4.c

clean: 
	rm -f new_intr no_echo test_partial_read test_tty_functions demo_SIGWINCH exercises62-1

all: new_intr no_echo test_partial_read

new_intr: error_functions.c new_intr.c
	gcc -o new_intr error_functions.c new_intr.c

no_echo: error_functions.c no_echo.c
	gcc -o no_echo error_functions.c no_echo.c

test_partial_read: test_partial_read.c
	gcc -o test_partial_read test_partial_read.c

clean: 
	rm -f new_intr no_echo test_partial_read

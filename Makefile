all:

%:
	make -C c $@ --no-print-directory
	make -C c++ $@ --no-print-directory
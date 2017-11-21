default: all

%:
	make -C c $@
	make -C c++ $@
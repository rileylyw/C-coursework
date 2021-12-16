CC := clang
CWARNS := -Wall -Wpedantic -Wextra -Wfloat-equal -Wvla
CFLAGNOABORT := $(CWARNS) -std=c99 -O3
CFLAGS := $(CFLAGNOABORT) -Werror
LDLIBS := -lm
SANI := -fsanitize=undefined -fsanitize=address
NLBS := $(wildcard *.nlb)
RESULTS := $(NLBS:.nlb=.result)


all : parse parse_s parse_v interp interp_s interp_v

interp : nlab.c nlab.h
	$(CC) nlab.c $(CFLAGS) -O2 -DINTERP -o interp $(LDLIBS) 

interp_s : nlab.c nlab.h
	$(CC) nlab.c $(CFLAGS) $(SANI) -g3 -DINTERP -o interp_s $(LDLIBS) 
	
interp_v : nlab.c nlab.h
	$(CC) nlab.c $(CFLAGS) -g3 -DINTERP -o interp_v $(LDLIBS) 


parse : nlab.c nlab.h
	$(CC) nlab.c $(CFLAGS) -O2 -o parse $(LDLIBS) 

parse_s : nlab.c nlab.h
	$(CC) nlab.c $(CFLAGS) $(SANI) -g3 -o parse_s $(LDLIBS) 
	
parse_v : nlab.c nlab.h
	$(CC) nlab.c $(CFLAGS) -g3 -o parse_v $(LDLIBS) 

runall : $(RESULTS)

%.result:
		./interp $*.nlb > $*.results


clean:
	rm -f parse parse_s parse_v interp interp_s interp_v $(RESULTS)
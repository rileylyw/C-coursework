CC := clang
CWARNS := -Wall -Wpedantic -Wextra -Wfloat-equal -Wvla
CFLAGNOABORT := $(CWARNS) -std=c99 -O3
CFLAGS := $(CFLAGNOABORT) #-Werror
LDLIBS := -lm
SANI := -fsanitize=undefined -fsanitize=address
NLBS := $(wildcard *.nlb)
RESULTS := $(NLBS:.nlb=.result)


all : parse parse_s parse_v interp interp_s interp_v test_parse test_parse_s test_interp test_interp_s test_interp_shell

interp : nlab.c nlab.h main.c
	$(CC) main.c nlab.c general.c $(CFLAGS) -O2 -DINTERP -o interp $(LDLIBS) 

interp_s : nlab.c nlab.h main.c
	$(CC) main.c nlab.c general.c $(CFLAGS) $(SANI) -g3 -DINTERP -o interp_s $(LDLIBS) 
	
interp_v : nlab.c nlab.h main.c
	$(CC) main.c nlab.c general.c $(CFLAGS) -g3 -DINTERP -o interp_v $(LDLIBS) 


parse : nlab.c nlab.h main.c
	$(CC) main.c nlab.c general.c $(CFLAGS) -O2 -o parse $(LDLIBS) 

parse_s : nlab.c nlab.h main.c
	$(CC) main.c nlab.c general.c $(CFLAGS) $(SANI) -g3 -o parse_s $(LDLIBS) 
	
parse_v : nlab.c nlab.h main.c
	$(CC) main.c nlab.c general.c $(CFLAGS) -g3 -o parse_v $(LDLIBS) 



test_parse : nlab.c nlab.h test_parse.c
	$(CC) test_parse.c nlab.c general.c $(CFLAGS) -O2 -DTEST -o test_parse $(LDLIBS) 
test_parse_s : nlab.c nlab.h test_parse.c
	$(CC) test_parse.c nlab.c general.c $(CFLAGS) $(SANI) -g3 -DTEST -o test_parse_s $(LDLIBS) 

test_interp : nlab.c nlab.h test_interp.c
	$(CC) test_interp.c nlab.c general.c $(CFLAGS) -O2 -DINTERP -DTEST -o test_interp $(LDLIBS) 
test_interp_s : nlab.c nlab.h test_interp.c
	$(CC) test_interp.c nlab.c general.c $(CFLAGS) $(SANI) -g3 -DINTERP -DTEST -o test_interp_s $(LDLIBS) 

test_interp_shell : nlab.c nlab.h main.c
	$(CC) main.c nlab.c general.c $(CFLAGS) -O2 -DINTERP -o Test_interp/interp $(LDLIBS) 



-k runall : $(RESULTS)

%.result:
		./interp $*.nlb > $*.results

clean:
	rm -f parse parse_s parse_v interp interp_s interp_v $(RESULTS)
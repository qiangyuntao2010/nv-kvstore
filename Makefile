include IncludePre.mk

SUB_DIRS = src test

all:
	@for dir in $(SUB_DIRS); do \
		make -C $$dir all; \
	done

clean:
	@for dir in $(SUB_DIRS); do \
		make -C $$dir clean; \
	done
	rm -f core.*
	rm -f gmon.out

depend:
	@for dir in $(SUB_DIRS); do \
		make -C $$dir depend; \
	done

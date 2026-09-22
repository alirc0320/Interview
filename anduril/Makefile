# Self-contained build for the anduril/ prep track. Run from inside
# anduril/, or `make -C anduril <target>` from the repo root.
#
# Pure C (not C++) — matches the actual interview format (live coding in C
# via HackerRank CodePair).

CC := cc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow \
	-g -O0 -fsanitize=address,undefined
INCLUDES := -Icommon

.PHONY: test new clean list new-topic help

test:
ifeq ($(PROB),)
	$(error Usage: make test PROB=<name>  (e.g. make test PROB=bitfield_extract_insert))
endif
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -Iproblems/$(PROB) \
		problems/$(PROB)/solution.c problems/$(PROB)/test.c \
		-o build/$(PROB)_test
	./build/$(PROB)_test

new:
ifeq ($(PROB),)
	$(error Usage: make new PROB=<name>)
endif
	@mkdir -p problems/$(PROB)
	@cp -n template/solution.h template/solution.c template/test.c problems/$(PROB)/
	@echo "Created problems/$(PROB)/ (solution.h, solution.c, test.c)"

list:
	@ls problems 2>/dev/null || echo "(no problems yet — run: make new PROB=<name>)"

new-topic:
ifeq ($(NAME),)
	$(error Usage: make new-topic NAME=<slug>  (e.g. make new-topic NAME=09_bootloaders))
endif
	@cp -n template_topic.md topics/$(NAME).md
	@echo "Created topics/$(NAME).md"

clean:
	rm -rf build

help:
	@echo "make new       PROB=<name>   scaffold a new problem from template/"
	@echo "make test      PROB=<name>   build & run problems/<name>/*.c"
	@echo "make list                    list existing problems"
	@echo "make new-topic NAME=<slug>   scaffold a new topics/<slug>.md"
	@echo "make clean                   remove build artifacts"

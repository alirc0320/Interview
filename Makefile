# Matches the Etched interview guide's recommended compile line, plus
# -Wshadow for good measure.
CXX := c++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -Wshadow \
	-g -O0 -fsanitize=address,undefined
INCLUDES := -Icommon

.PHONY: test new clean list help new-chapter new-section new-practice practice format

test:
ifeq ($(PROB),)
	$(error Usage: make test PROB=<name>  (e.g. make test PROB=two_sum))
endif
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(INCLUDES) -Iproblems/$(PROB) \
		problems/$(PROB)/solution.cpp problems/$(PROB)/test.cpp \
		-o build/$(PROB)_test
	./build/$(PROB)_test

new:
ifeq ($(PROB),)
	$(error Usage: make new PROB=<name>  (e.g. make new PROB=valid_parens))
endif
	@mkdir -p problems/$(PROB)
	@cp -n template/solution.h template/solution.cpp template/test.cpp problems/$(PROB)/
	@echo "Created problems/$(PROB)/ (solution.h, solution.cpp, test.cpp)"

list:
	@ls problems 2>/dev/null || echo "(no problems yet — run: make new PROB=<name>)"

new-chapter:
ifeq ($(NAME),)
	$(error Usage: make new-chapter NAME=<slug>  (e.g. make new-chapter NAME=11_arrays))
endif
	@cp -n learncpp/template_chapter.md learncpp/chapters/$(NAME).md
	@echo "Created learncpp/chapters/$(NAME).md"

new-practice:
ifeq ($(NAME),)
	$(error Usage: make new-practice NAME=<slug>  (e.g. make new-practice NAME=11_12))
endif
	@cp -n learncpp/practice/template.cpp learncpp/practice/$(NAME).cpp
	@echo "Created learncpp/practice/$(NAME).cpp"

practice:
ifeq ($(NAME),)
	$(error Usage: make practice NAME=<slug>)
endif
	@mkdir -p build
	$(CXX) $(CXXFLAGS) learncpp/practice/$(NAME).cpp -o build/practice_$(NAME)
	./build/practice_$(NAME)

new-section:
ifeq ($(NAME),)
	$(error Usage: make new-section NAME=<slug>  (e.g. make new-section NAME=ostep_26_concurrency))
endif
	@cp -n os/template_section.md os/sections/$(NAME).md
	@echo "Created os/sections/$(NAME).md"

format:
	@./format.sh

clean:
	rm -rf build

help:
	@echo "make new  PROB=<name>          scaffold a new problem from template/"
	@echo "make test PROB=<name>          build & run its tests"
	@echo "make list                      list existing problems"
	@echo "make format                    format all C++ files with clang-format"
	@echo "make clean                     remove build artifacts"
	@echo ""
	@echo "make new-chapter  NAME=<slug>   scaffold a LearnCpp chapter log"
	@echo "make new-practice NAME=<slug>   scaffold a combined LearnCpp practice.cpp"
	@echo "make practice     NAME=<slug>   build & run learncpp/practice/<slug>.cpp"
	@echo "make new-section  NAME=<slug>   scaffold an OS chapter/section log"

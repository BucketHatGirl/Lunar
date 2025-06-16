LIBS := Libraries
SRC := Source
SOURCE_FILES := $(shell find $(SRC) -type f -name '*.cpp') 
FLAGS := -O2 -std=c++23 -Wall -lz -L$(LIBS) -llua -lminiz -I$(SRC)/Headers/

.PHONY: all
all: Libraries Source Clean

.PHONY: Libraries
Libraries:
	@clear
	@echo "Compiling libraries...\n"
	@clang -c $(LIBS)/Lua/*.c
	@ar rcs liblua.a *.o
	@rm -f *.o
	@clang -c $(LIBS)/Miniz/*.c
	@ar rcs libminiz.a *.o
	@rm -f *.o
	@mv *.a $(LIBS)

.PHONY: Source
Source:
	@clear
	@echo "Compiling source...\n"
	@clang++ $(FLAGS) $(SOURCE_FILES) -o Lunar
	@rm -f $(shell find . -type f -name '*.o')

.PHONY: Clean
Clean:
	@rm -f $(shell find . -type f -name '*.o')

.PHONY: Install
Install:
	@install ./Lunar $(shell dirname ~)/usr/bin

CC = g++
LINUXFLAGSDEBUG = -D _DEBUG -ggdb3 -O0 -std=c++17 -Wall -Wextra -Weffc++ \
	-Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations \
	-Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion \
	-Wctor-dtor-privacy -Wempty-body -Wformat-nonliteral -Wformat-security \
	-Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd \
	-Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow \
	-Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 \
	-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand \
	-Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers \
	-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector \
	-fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=580192 -Wstack-usage=8192 -pie -fPIE -Werror=vla \
	-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,$\
	nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,$\
	signed-integer-overflow,undefined,unreachable,vla-bound,vptr -mavx2

LINUXFLAGRELIZE = -O3 -std=c++17 -Wall -Wextra -march=native -mavx2 -ggdb3


BIN_DIR = ./build/bin

OBJ_DIR = ./build/obj

./sfml-app: $(OBJ_DIR)/main.o $(OBJ_DIR)/MbFractal.o
	@$(CC) $(OBJ_DIR)/main.o $(OBJ_DIR)/MbFractal.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system $(LINUXFLAGSDEBUG)

$(OBJ_DIR)/main.o: main.cpp Mandelbrot/MbFractal.h
	@$(CC) -c main.cpp -o $(OBJ_DIR)/main.o $(LINUXFLAGSDEBUG)

$(OBJ_DIR)/MbFractal.o: Mandelbrot/MbFractal.cpp Mandelbrot/MbFractal.h
	@$(CC) -c Mandelbrot/MbFractal.cpp -o $(OBJ_DIR)/MbFractal.o $(LINUXFLAGSDEBUG)

clean:
	rm -f main $(OBJ_DIR)/*.o

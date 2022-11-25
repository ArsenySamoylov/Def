CC   := g++
NAME := def
ARGS := 

LOGFILE := compileLog

CFLAGS  := -g   -D _DEBUG  -std=c++20 		-Wall  -Wc++14-compat  				-Wextra  				-Wcast-qual    				-Wcast-align   					   \
				-Weffc++   -Wfloat-equal 	-Wchar-subscripts  					-Wconversion   			-Woverloaded-virtual   		-Wstack-protector				   \
				-Wformat-nonliteral    		-Wempty-body     					-Wformat-security       -Wformat=2	   				-Wformat-signedness	   			   \
			    -Wlogical-op           		-Wopenmp-simd    					-Wnon-virtual-dtor 	    -Winline	   				-Wconditionally-supported          \
				-Wpacked   -Wpointer-arith  -Winit-self       	 				-Wredundant-decls       -Wshadow 					-Wmissing-declarations 			   \
				-Wsign-conversion      		-Wsign-promo     					-Wstrict-null-sentinel  -Wstrict-overflow=2			-Wsuggest-attribute=noreturn  	   \
				-Wsuggest-final-methods     -Wsuggest-final-types 				-Wundef 				-Wctor-dtor-privacy 		-Waggressive-loop-optimizations    \
				-Wsuggest-override     		-Wswitch-default 					-Wswitch-enum           -Wsync-nand    				-flto-odr-type-merging 			   \
				-Wunreachable-code     		-Wunused         					-Wuseless-cast 		    -Wvariadic-macros 			-fstrict-overflow 				   \
				-Wno-literal-suffix    		-Wno-missing-field-initializers		-Wno-narrowing 			-Wno-old-style-cast 		-fstack-protector				   \
				-Wno-varargs           		-fcheck-new     					-fsized-deallocation    -fno-omit-frame-pointer 	-Wpedantic 						   \
				-Wlarger-than=8192     		-Wstack-usage=8192 					-pie -fPIE 				
							   		 					

SANITIZERS := -fsanitize=address,leak #,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
LFLAGS 	   := -lpthread  -lasan
			  # -lsfml-window -lsfml-system -lsfml-graphics -lncurses -lsfml-audio 

SRCDIR :=  ./src     ./ATC 
SRCDIR := $(shell find $(SRCDIR) -type d)

INCDIR :=  ./headers ./ATC 
INCDIR := $(shell find $(INCDIR) -type d)

OBJDIR := build
DEPDIR := dependences

SOURCES     := $(wildcard $(addsuffix /*.cpp, $(if $(SRCDIR), $(SRCDIR), .)) )

OBJECTS     := $(patsubst %.cpp, $(if $(OBJDIR), $(OBJDIR)/%.o, ./%.o), $(notdir $(SOURCES)) )
DEPENDENCES := $(patsubst %.cpp, $(if $(DEPDIR), $(DEPDIR)/%.d, ./%.d), $(notdir $(SOURCES)) )

VPATH := $(SRCDIR)

.PHONY: clean cleanLog run  dependences cleanDependences makeDependencesDir objects check openLog

$(NAME):  dependences objects $(OBJECTS) cleanDependences
	@$(if $(OBJECTS), $(CC) $(OBJECTS) $(LFLAGS) -o $@ #2>>$(LOGFILE))

############################################################################
clean:
	@rm -rf $(OBJECTS) $(DEPENDENCES) $(DEPDIR) $(NAME)  #$(shell ls build/*.ii -t | head -1) $(shell ls build/*.s -t | head -1)

cleanLog:
	@rm -rd logs/*.ars

openLog:
	@xdg-open $(shell ls Logs/*.html -t | head -1)

############################################################################
check: clean $(NAME)
	@$(if $(NAME), valgrind --leak-check=full -s\
         --show-leak-kinds=all                \
         ./$(NAME) $(ARGS))

run: clean $(NAME)
	@$(if $(NAME), ./$(NAME) $(ARGS))

#############################################################################
dependences: makeDependencesDir $(DEPENDENCES)

makeDependencesDir:
	@$(if $(DEPDIR), mkdir -p $(DEPDIR))

$(if $(DEPDIR), $(DEPDIR)/%.d, %.d): %.cpp
	@$(CC) -M $(addprefix -I, $(INCDIR)) $< -o $@

cleanDependences:
	@rm -rf $(DEPENDENCES) $(DEPDIR)

##############################################################################
objects:
	@$(if $(OBJDIR), mkdir -p $(OBJDIR))

$(if $(OBJDIR), $(OBJDIR)/%.o, %.o): %.cpp
	@$(CC) -c $(addprefix -I, $(INCDIR)) -save-temps $(CFLAGS) $(SANITIZERS) $< -o $@ #2>>$(LOGFILE)

include $(wildcard $(DEPDIR)/*.d)

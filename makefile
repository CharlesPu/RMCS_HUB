EXEC = r
DIR_CURR=./
DIR_OBJ=./OBJ/

CPP_SRCS  =$(wildcard ${DIR_CURR}*.cpp)
CPP_SRCS +=$(wildcard ${DIR_CURR}app/*.cpp) $(wildcard ${DIR_CURR}infra/*.cpp) 
CPP_SRCS +=$(wildcard ${DIR_CURR}pipe/*.cpp) $(wildcard ${DIR_CURR}protocol/*.cpp)
		
CPP_OBJS = ${patsubst %.cpp,${DIR_OBJ}%.o, ${notdir ${CPP_SRCS}}}
CPP_SRCS_DIR = ${dir $(CPP_SRCS)}

# delete the repeatition
uniq 		= $(eval seen :=)$(foreach i,$1,$(if $(filter $i,${seen}),,$(eval seen += $i)))${seen}
rwildcard	= $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
ALL_DIRS 	= $(dir $(foreach src_path,$(DIR_CURR), $(call rwildcard,$(src_path),*/) ) )
ALL_DIRS 	:= $(call uniq, $(ALL_DIRS))
INC_DIR 	:= $(ALL_DIRS)
# libraries
A_LIB_DIR =
LIBS_A	 =

LIB_DIR 	:= $(A_LIB_DIR)
LIBS 		:= $(LIBS_A)

CC = g++
CFLAGS = -O2 -Wall -g `mysql_config --cflags` -std=c++11
INCFLAGS = $(foreach i, $(INC_DIR),-I$(i))
LDFLAGS += $(foreach i, $(LIB_DIR),-L$(i))
LDFLAGS += $(foreach i, $(LIBS),-l$(i))
SQL_LINK_FLAGS = `mysql_config --libs`
MTHREADFLAGS = -pthread
OMPFLAGS = -fopenmp

all:clean $(EXEC)

$(EXEC):$(CPP_OBJS)
	$(CC) -o $@ $(CPP_OBJS) $(MTHREADFLAGS) $(SQL_LINK_FLAGS) $(OMPFLAGS)
	@echo "Make Done~"

VPATH = $(CPP_SRCS_DIR)
${DIR_OBJ}%.o:${DIR_CURR}%.cpp	
	@if [ ! -d $(DIR_OBJ) ]; then mkdir -p $(DIR_OBJ); fi; 
	@$(CC) -c $< -o $@ $(CFLAGS) $(INCFLAGS) $(MTHREADFLAGS) $(OMPFLAGS)

.PHONY:clean
clean:
	@rm -rf ${DIR_OBJ}
	@rm -rf ${EXEC}
	@echo "Clean Done~"

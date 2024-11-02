CC = gcc
FLAGS = -Wall -Wextra -MMD -MP ${flags}
DIRS = include utils
SRC_DIR = src/
INC_DIRS = ${addprefix -I, ${DIRS}}

OUTPUT_DIR =build/
OUTPUT_DEPS=build/
TARGET=${OUTPUT_DIR}lang

FILES_TO_COMPILE = ${foreach _D, ${SRC_DIR},${wildcard ${_D}*.c}}
OUTPUT_FILES_NAME = ${patsubst %.c, ${OUTPUT_DIR}%.o, ${FILES_TO_COMPILE}}
DEPS=${patsubst %.c, ${OUTPUT_DEPS}%.d, ${FILES_TO_COMPILE}}

all: ${TARGET}

test: directories ${OUTPUT_FILES_NAME}
	${CC} ${FLAGS} ${OUTPUT_FILES_NAME} ${tf} ${INC_DIRS} -o ${OUTPUT_DIR}test

${TARGET}: directories ${OUTPUT_FILES_NAME}
	${CC} ${FLAGS} ${OUTPUT_FILES_NAME} main.c ${INC_DIRS} -o ${TARGET}

${OUTPUT_DIR}${SRC_DIR}%.o: ${SRC_DIR}%.c 
	${CC} ${FLAGS} ${INC_DIRS} -c $< -o $@

${OUTPUT_DIR}%.o: %.c
	${CC} ${FLAGS} ${INC_DIRS} -c $< -o $@

directories:
	mkdir -p ${OUTPUT_DIR}
	${foreach f, ${SRC_DIR}, ${shell mkdir -p ${OUTPUT_DIR}${f}}}

clean:
	rm -rf ${OUTPUT_DIR}

.PHONY: all clean directories test

-include $(DEPS)
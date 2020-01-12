CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = q4target
OBJECTS = re.o q4target.o core.o

DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECT} ${DEPENDS}

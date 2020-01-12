CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = a3q1
OBJECTS = re.o q1main.o

DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECT} ${DEPENDS}

CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = a3q2
OBJECTS = re.o q2main.o

DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECT} ${DEPENDS}

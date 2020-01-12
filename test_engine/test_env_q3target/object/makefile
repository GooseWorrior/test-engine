CXX = g++
CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = q3target
OBJECTS = re.o core.o q3target.o

DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS}

CC       = c++
FLAGS    =
CFLAGS   = -g -O2
DFLAGS   = -DPACKAGE=\"threadsystem\" -DVERSION=\"1.0\" -DPTHREAD=1 -DUNIX_MSG_Q=1 -DSTDC_HEADERS=1
IFLAGS   = -I/home/dan/Documents/ThreadMentor/include
TMLIB    = /home/dan/Documents/ThreadMentor/Visual/libthreadclass.a
TMLIB_NV = /home/dan/Documents/ThreadMentor/NoVisual/libthreadclass.a

OBJ_FILE = thread.o thread-main.o
EXE_FILE = prog3

${EXE_FILE}: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB} -lpthread

thread.o: thread.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread.cpp

thread-main.o: thread-main.cpp
	${CC} ${DFLAGS} ${IFLAGS} ${CFLAGS} -c thread-main.cpp

noVisual: ${OBJ_FILE}
	${CC} ${FLAGS}  -o ${EXE_FILE}  ${OBJ_FILE} ${TMLIB_NV} -lpthread

clean:
	rm -f ${OBJ_FILE} ${EXE_FILE}

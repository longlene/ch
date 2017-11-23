CXXFLAGS=-std=c++11 -I../include
LDFLAGS=-pthread

all: chan_test buffered unbuffered job

chan_test.o: ../include/chan.hpp
chan_test: chan_test.o
	$(CXX) -o $@ $<

buffered.o: ../include/chan.hpp
buffered: buffered.o
	$(CXX) -o $@ $<

unbuffered.o: ../include/chan.hpp
unbuffered: unbuffered.o
	$(CXX) -o $@ $< $(LDFLAGS)

job.o: ../include/chan.hpp
job: job.o
	$(CXX) -o $@ $< $(LDFLAGS)

clean:
	-rm -rf *.o chan_test buffered unbuffered job

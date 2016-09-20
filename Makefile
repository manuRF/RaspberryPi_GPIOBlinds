all: persianas
persianas:  persianas.o gpio.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $+ -o $@ -lwiringPi -lrt -llirc_client 
clean:
	$(RM) *.o

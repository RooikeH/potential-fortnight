.SUFFIXES: .c .o  
  
CC=gcc  
ASRCS=client.c              
BSRCS=server.c  
      
AOBJS=$(ASRCS:.c=.o)  
BOBJS=$(BSRCS:.c=.o)  
  
AEXEC=client  
BEXEC=server  
  
all:$(AOBJS) $(BOBJS)  
	$(CC) -o $(AEXEC) $(AOBJS)  
	$(CC) -o $(BEXEC) $(BOBJS)  
	@echo '-------------ok--------------'  
  
.c.o:  
	$(CC) -Wall -g -o $@ -c $<   
  
clean:  
	rm -f $(AOBJS)  
	rm -f $(BOBJS)  
	rm -f core*  
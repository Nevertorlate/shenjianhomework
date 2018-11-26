# target = client
 
# .PHONY :all
# all: $(target) 

# client:utility.cpp client.cpp utility.h
# 	g++ -o client client.cpp utility.cpp utility.h -std=c++11

# utility.o:utility.cpp utility.h
# 	g++ -c utility.o utility.cpp utility.h -std=c++11

# .PHONY: clean
# clean:
# 	rm -rf *.o $(target)

src = utility.cpp client.cpp server.cpp client_nofork_nonblock.cpp server_nofork_nonblock.cpp
dep = utility.h common.h
obj = $(src:%.cpp=%.o)

target = client server

.PHONY :all m
all: $(target) 

client:
	g++ -o client client.cpp  utility.cpp client_nofork_nonblock.cpp nofork_nonblock.cpp -std=c++11

server:
	g++ -o server server.cpp  utility.cpp server_nofork_nonblock.cpp nofork_nonblock.cpp -std=c++11
m:
	g++ -o client client.cpp  utility.cpp client_nofork_nonblock.cpp nofork_nonblock.cpp -std=c++11
	g++ -o server server.cpp  utility.cpp server_nofork_nonblock.cpp nofork_nonblock.cpp -std=c++11

# %.o: %.c $(deps)
# 	g++ -c $< -o $@  $(dep)  -std=c++11

clean:
	rm -rf $(obj) $(target)
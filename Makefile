NAME = webserv
SRC =	./server.cpp\
		./parsing/Location.cpp\
		./parsing/Server.cpp\
		./parsing/parsing.cpp\
		./parsing/tools.cpp\
		./server/socket.cpp\
		./server/helper.cpp\

HEADERS = parsing/Location.hpp\
		request_response/request.hpp\
		request_response/response.hpp\
		parsing/Server.hpp\
		request_response/request_handler.hpp\
		request_response/ARequest.hpp\
		request_response/DELETERequest.hpp\
		request_response/GETRequest.hpp\
		request_response/POSTRequest.hpp\
		server/socket.hpp


all: $(NAME) $(HEADERS)

$(NAME): $(SRC) $(HEADERS)
	@clang++  $(SRC) -o $(NAME)

clean:
	@rm -f $(NAME)

fclean: clean
	@rm -f $(NAME)

re: fclean $(NAME)
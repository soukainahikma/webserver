

CC = clang++
# CFLAGS = -Wall -Wextra -Werror -std=c++98
NAME = webserv
SRC =	./server.cpp\
		./parsing/Location.cpp\
		./parsing/Server.cpp\
		./parsing/parsing.cpp\
		./parsing/tools.cpp\
		./parsing/cgi.cpp\
		./server/socket.cpp\
		./server/helper.cpp\

INC = parsing/Location.hpp\
		request_response/request.hpp\
		request_response/response.hpp\
		parsing/Server.hpp\
		request_response/request_handler.hpp\
		request_response/ARequest.hpp\
		request_response/DELETERequest.hpp\
		request_response/GETRequest.hpp\
		request_response/POSTRequest.hpp\
		server/socket.hpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@echo "\n\033[36m◊◊◊ Producin' object files done! ◊◊◊\033[0m\n"
	@$(CC) $(OBJ) -o $(NAME)
	@echo "\n\033[32m◊◊◊ ------- $(NAME) has been created successfully ------- ◊◊◊\033[0m\n"

%.o: %.cpp $(INC)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ)
	@echo "\n\033[36m◊◊◊ Cleanin' object files done! ◊◊◊\033[0m\n"

fclean: clean
	@rm -rf $(NAME)

re: fclean all
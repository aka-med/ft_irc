NAME = ircserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g
VERSION = -std=c++98 -I$(INCS_DIR)

SRCS_DIR = srcs/
INCS_DIR = includes/
SRCS_CMD_DIR = srcs/Command/
OBJS_DIR = obj/

INCS = $(wildcard $(INCS_DIR)*.hpp)
SRCS = $(wildcard $(SRCS_DIR)*.cpp) $(wildcard $(SRCS_CMD_DIR)*.cpp)
OBJS = $(patsubst %.cpp, $(OBJS_DIR)%.o, $(notdir $(SRCS)))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(VERSION) $(OBJS) -o $(NAME)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp $(INCS) | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(VERSION) -c $< -o $@

$(OBJS_DIR)%.o: $(SRCS_CMD_DIR)%.cpp $(INCS) | $(OBJS_DIR)
	$(CXX) $(CXXFLAGS) $(VERSION) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJS)
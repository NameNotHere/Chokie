# Compiler
CXX     = g++
CXXFLAGS= -Wall -Wextra -Werror -std=c++17

# Project name
NAME    = chookie

# Source files
SRCS    = main.cpp draw.cpp files.cpp \
          input_helpers.cpp input_modes.cpp

OBJS    = $(SRCS:.cpp=.o)

# Raylib flags (Linux default)
LIBS    = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Default rule
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(OBJS) -o $(NAME) $(LIBS)

%.o: %.cpp input.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

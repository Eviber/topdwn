NAME = topdwn 
CC = gcc
CFLAGS = -Wall -Wextra -Werror `sdl2-config --cflags --libs` -lSDL2_image

SRC = topdwn.c
OBJS_FILES = ${SRC:c=o}
OBJS_DIR = objs
OBJ = $(OBJS_DIR)/$(OBJS_FILES)

all: $(NAME)

$(OBJS_DIR)/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS)	-c -o $@ $^

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

clean:
	@rm -rf $(OBJS_FILES)
	@echo "Object files removed."

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME) removed."

re: fclean all

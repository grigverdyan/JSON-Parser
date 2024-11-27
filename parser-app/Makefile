.PHONY : all clean fclean re

TARGET			= json_parser
CC				= cc
CFLAGS			= -Wall -Wextra -Werror #g
RM				= rm -rf
MKDIR			= mkdir -p

HEADERS_DIR		= headers
HEADERS_FLAG	= -I$(HEADERS_DIR)
HEADERS			= $(HEADERS_DIR)/*.h

SRC_DIR			= sources
SRC_FILES		= $(wildcard $(SRC_DIR)/*.c)

OBJS			= $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
OBJ_DIR			= objs

# Colors
RESET			= "\033[0m"
BLACK    		= "\033[30m"    # Black
RED      		= "\033[31m"    # Red
GREEN    		= "\033[32m"    # Green
YELLOW   		= "\033[33m"    # Yellow
BLUE     		= "\033[34m"    # Blue
MAGENTA  		= "\033[35m"    # Magenta
CYAN     		= "\033[36m"    # Cyan
WHITE    		= "\033[37m"    # White

all : $(TARGET) 

$(TARGET) : $(OBJS)
	@echo $(GREEN) "  Creating $(TARGET)" $(RESET)
	@$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)	

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(HEADERS_FLAG) -c $< -o $@

$(OBJ_DIR) :
	@$(MKDIR) $(OBJ_DIR)

clean :
	@$(RM) $(OBJS) 
	@echo $(MAGENTA) "  Removing Object Files" $(RESET)

fclean : clean
	@$(RM) $(TARGET)
	@$(RM) $(OBJ_DIR)
	@echo $(RED) "  Removing $(TARGET)" $(RESET)

re : fclean all
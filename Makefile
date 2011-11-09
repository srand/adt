CC 	= gcc
AR 	= ar
MKDIR	= mkdir -p
RM	= rm -rf
CFLAGS	= -Wall -Wextra -Werror -pedantic -std=c99 -Iinclude
SILENCE = @
OBJDIR	= objs

OBJS	+= list.o
OBJS	+= slist.o
OBJS	+= rbtree.o
OBJS	+= hash.o

OBJS	:= $(addprefix $(OBJDIR)/,$(OBJS))

vpath	%.c src


all: libadt.a

libadt.a: $(OBJDIR) $(OBJS)
	@echo "  AR " $@
	$(SILENCE)$(AR) cr $@ $(OBJS)

$(OBJDIR): 
	$(SILENCE)$(MKDIR) $@

$(OBJDIR)/%.o: src/%.c
	@echo "  CC " $@
	$(SILENCE)$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(SILENCE)$(RM) $(OBJDIR) libadt.a

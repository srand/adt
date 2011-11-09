CC 	= gcc
AR 	= ar
MKDIR	= mkdir -p
RM	= rm -rf
CFLAGS	= -Wall -Wextra -Werror -pedantic -std=c99 -Iinclude
SILENCE = @
SRCDIR	= src
OBJDIR	= objs

ifeq ($(VERBOSE),yes)
SILENCE :=
endif

OBJS	+= list.o
OBJS	+= slist.o
OBJS	+= rbtree.o
OBJS	+= hash.o

OBJS	:= $(addprefix $(OBJDIR)/,$(OBJS))
vpath	%.c $(SRCDIR)


all: libadt.a

libadt.a: $(OBJDIR) $(OBJS)
	@echo "  AR " $@
	$(SILENCE)$(AR) cr $@ $(OBJS)

$(OBJDIR): 
	$(SILENCE)$(MKDIR) $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "  CC " $@
	$(SILENCE)$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(SILENCE)$(RM) $(OBJDIR) libadt.a

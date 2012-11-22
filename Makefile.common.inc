CC = $(PREFIX)-gcc
CXX = $(PREFIX)-g++
GDB = $(PREFIX)-gdb
OBJCOPY = $(PREFIX)-objcopy
OBJDUMP = $(PREFIX)-objdump
AR  = $(PREFIX)-ar
SIZE = $(PREFIX)-size
NM = $(PREFIX)-nm
REMOVE = rm -f
MV = mv -f

DEBUG =

# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
COPT = s
CSTANDARD = -std=gnu99
CDEBUG = -g$(DEBUG)
CWARN = -Wno-main -Wall -Wstrict-prototypes
CTUNING = -Wall -O$(COPT) -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields \
-fno-exceptions -ffunction-sections -fdata-sections

CDEFS = -mmcu=$(MCU)
CXXDEFS = -mmcu=$(MCU)
CFLAGS = -Wa,-adhlns=$(<:.c=.lst) $(CDEBUG) $(CDEFS) $(CINCS) $(CTUNING) $(CWARN) $(CSTANDARD) $(CEXTRA)
CXXFLAGS = -Wa,-adhlns=$(<:.cpp=.lst) $(CDEBUG) $(CXXDEFS) $(CINCS) $(CTUNING) $(CXXWARN) $(CXXEXTRA)

ifeq ($(LDSCRIPT),)
LDSCRIPTFLAGS=
else
LDSCRIPTFLAGS=-T $(LDSCRIPT)
endif

LDFLAGS = $(LDSCRIPTFLAGS) -Wl,--gc-sections,-Map=$(TARGET).map $(LDEXTRA)
ASFLAGS = $(CDEFS) -x assembler-with-cpp -Wa,-adhlns=$(<:.S=.lst),-g$(DEBUG)

# Define all object files.
OBJ = $(SRC:.c=.o) $(ASRC:.S=.o) $(CXXSRC:.cpp=.o)

# Define all listing and map files.
LST = $(SRC:.c=.lst) $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst)
MAPS = $(TARGET).map

all: $(TARGET).elf

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $<


# Compile: create object files from C source files.
.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ASFLAGS) $< -o $@

# Compile: create object files from C++ source files.
.cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@

# Compile: create assembler files from C source files.
.c.s:
	$(CC) -S $(CFLAGS) $< -o $@

clean:
	$(REMOVE) $(OBJ) $(TARGET).elf $(LST) $(MAPS) $(CLEAN)

size: $(TARGET).elf
	$(SIZE) -A --common $<


.PHONY: all upload flash clean size
.PRECIOUS: .elf

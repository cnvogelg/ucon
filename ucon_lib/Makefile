# AmigaOS library header generation helper

BASE=ucon
ARCH=m68k

# the tool
FD2PRAGMA=fd2pragma

# the source file
SFD=$(BASE)_lib.sfd

# derived files with fd2pragma
INLINE=include/inline/$(BASE)_protos.h
PROTO=include/proto/$(BASE).h
CLIB=include/clib/$(BASE)_protos.h

all: $(INLINE) $(PROTO) $(CLIB)

clean:
	rm -f $(INLINE) $(PROTO)

$(INLINE): $(SFD)
	$(FD2PRAGMA) -a $(ARCH) -s 70 -t include/inline -i $(SFD)

$(PROTO): $(SFD)
	$(FD2PRAGMA) -a $(ARCH) -s 38 -t include/proto -i $(SFD)

$(CLIB): $(SFD)
	$(FD2PRAGMA) -a $(ARCH) -s 111 -t include/clib -i $(SFD)

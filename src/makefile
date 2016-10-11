CC		= ccpsx
ASM		= asmpsx
LINK	= slink
LIBLNK	= psylib2
GTEFIX	= dmpsx

ifeq ($(STRIP),STRIP)
STRIP_SYM=/st /strip
else
STRIP_SYM=
endif

CCOPTIONS  = -O$(OPTIMIZE) -mgpopt -G0 -comments-c++ -nostdlib -c -D$(PLAT) -D$(RELMODE) -I.. -I.
CCOPTIONX  = -O1 -mgpopt -G0 -comments-c++ -nostdlib -c -D$(PLAT) -D$(RELMODE) -I.. -I.
ASMOPTIONS = /l /o c+ /o an+ /zd /x
LINKOPTS = /psx $(STRIP_SYM) /c /li $(RELMODE) /z /st /strip

## sources ##
CORE_SRC	= main.c gsrep.c vram.c fsprt.c font.c controller.c cd.c sound.c
3D_SRC		= gte.c render.c tm2.c particle.c
DEMO_SRC	= demo.c model.c
CORE_S		= datafile.asm

## objects ##
CORE_OBJ	= $(patsubst %.c,	$(RELMODE)/%.obj,	${CORE_SRC}		)
3D_OBJ		= $(patsubst %.c,	$(RELMODE)/%.obj,	${3D_SRC}		)
DEMO_OBJ	= $(patsubst %.c,	$(RELMODE)/%.obj,	${DEMO_SRC}		)
CORE_ASM	= $(patsubst %.asm,	$(RELMODE)/%.obj,	${CORE_S}		)

ALL_OBJ = $(CORE_ASM) $(3D_OBJ) $(CORE_OBJ) $(DEMO_OBJ)
## ------- ##

## compile directives ##
$(RELMODE)/%.obj : core/%.c
	$(CC) $(CCOPTIONS) $< -o $@
$(RELMODE)/%.obj : gte/%.c
	$(CC) $(CCOPTIONS) $< -o $@
	$(GTEFIX) -b $@
$(RELMODE)/%.obj : demo/%.c
	$(CC) $(CCOPTIONS) $< -o $@
$(RELMODE)/%.obj : core/%.asm
	$(ASM) $(ASMOPTIONS) $<,$@
## ----------------- ##

all: inform $(RELMODE)/${OUTFILE}.cpe

inform:
ifneq ($(RELMODE),RELEASE)
ifneq ($(RELMODE),DEBUG)
	@echo "Invalid configuration "$(RELMODE)" specified."
	@echo "You must specify a configuration when running make, e.g."
	@echo  "make RELMODE=DEBUG"
	@echo  
	@echo  "Possible choices for configuration are 'RELEASE' and 'DEBUG'"
	@exit 1
endif
endif
	@echo "Configuration "$(RELMODE)
	@echo "------------------------"

$(RELMODE)/${OUTFILE}.cpe: ${ALL_OBJ} | inform
	$(LINK) $(LINKOPTS) @$(OUTFILE).lk,$@,$(RELMODE)/${OUTFILE}.sym,${OUTFILE}.map
	cpe2psx $(RELMODE)\${OUTFILE}.cpe D:\ePSXe150\pSX\demo\${OUTFILE}.psx
	del $(RELMODE)\${OUTFILE}.cpe

clean: inform
	rmdir /q /s $(RELMODE)

rebuild: clean | all

# DO NOT DELETE
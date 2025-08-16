# Set up variables

# If $VERILATOR_ROOT isn't in the environment, we assume it is part of a
# package install, and verilator is in your path. Otherwise find the
# binary relative to $VERILATOR_ROOT (such as when inside the git sources).
ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
# VERILATOR_COVERAGE = verilator_coverage
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
# VERILATOR_COVERAGE = $(VERILATOR_ROOT)/bin/verilator_coverage
endif
IMG ?= image/$(ALL)-$(ARCH).bin
ELF ?= image/$(ALL)-$(ARCH).elf
ALL ?= dummy
ARCH ?= riscv32e-npc
ifeq ($(CONFIG_DIFFTEST),)
ARGS ?= --log=$(BUILD_DIR)/npc-log.txt
else
ARGS ?= --log=$(BUILD_DIR)/npc-log.txt --diff=image/riscv32-nemu-interpreter-so
endif
# Generate C++ in executable form
VERILATOR_FLAGS += -cc --exe
#VERILATOR_FLAGS += --dpi-c
# Generate makefile dependencies (not shown as complicates the Makefile)
#VERILATOR_FLAGS += -MMD
# Optimize
VERILATOR_FLAGS += -x-assign fast
# Warn abount lint issues; may not want this on less solid designs
VERILATOR_FLAGS += -Wall
CFLAGS += -I$(abspath include)
VERILATOR_FLAGS += -CFLAGS "$(CFLAGS)"
$(info VERILATOR_FLAGS = $(VERILATOR_FLAGS))
LDFLAGS += $(LIBS)
VERILATOR_FLAGS += --LDFLAGS "$(LDFLAGS)"
# Make waveforms
VERILATOR_FLAGS += --trace
VERILATOR_FLAGS += --trace-fst
# Check SystemVerilog assertions
VERILATOR_FLAGS += --assert
VERILATOR_FLAGS += --Wno-fatal
# Generate coverage analysis
# VERILATOR_FLAGS += --coverage
# Run Verilator in debug mode
#VERILATOR_FLAGS += --debug
# Add this trace to get a backtrace in gdb
#VERILATOR_FLAGS += --gdbbt
DESIGN = Top
VERILATOR_FLAGS += --top-module $(DESIGN)
VERILATOR_INPUT := 	$(SRCS) \
					$(shell find vsrc -name "*.sv") \
                   	build/$(DESIGN).sv
#VERILATOR_INPUT := 	$(shell find csrc -name "*.c") \
#					$(shell find vsrc -name "*.sv") \
#                  	csrc/sim_main.cpp build/$(DESIGN).sv

all:
	@echo "Write this Makefile by your self."

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by your self."
	@echo
	@echo "-- VERILATE ----------------"
	@cp $(NEMU_HOME)/build/riscv32-nemu-interpreter-so image/
	@cp $(AM_HOME)/../am-kernels/tests/cpu-tests/build/$(ALL)-$(ARCH).elf image/
	$(VERILATOR) $(VERILATOR_FLAGS) $(VERILATOR_INPUT)

	@echo
	@echo "-- BUILD -------------------"
# To compile, we can either
# 1. Pass --build to Verilator by editing VERILATOR_FLAGS above.
# 2. Or, run the make rules Verilator does:
	$(MAKE) -j -C obj_dir -f V$(DESIGN).mk
# 3. Or, call a submakefile where we can override the rules ourselves:
#	$(MAKE) -j -C obj_dir -f ../Makefile_obj

	@echo
	@echo "-- RUN ---------------------"
#	$(info ALL = $(ALL))
#	$(info ARCH = $(ARCH))
#	@rm elf/*
#	@mkdir elf/
#	@cp $(AM_HOME)/../am-kernels/tests/cpu-tests/build/$(ALL)-$(ARCH).elf image/
#	@cp $(NEMU_HOME)/build/riscv32-nemu-interpreter-so image/
#	obj_dir/V$(DESIGN) +trace
#	obj_dir/V$(DESIGN)
#	$(info IMG = $(IMG))
#	$(info ELF = $(ELF))
	obj_dir/V$(DESIGN) $(ARGS) $(IMG) $(ELF)

#	@echo
#	@echo "-- COVERAGE ----------------"
#	@rm -rf logs/annotated
#	$(VERILATOR_COVERAGE) --annotate logs/annotated logs/coverage.dat

	@echo
	@echo "-- DONE --------------------"
	@echo "To see waveforms, make wave"
	@echo


######################################################################
# Other targets

wave:
	gtkwave dump.fst

show-config:
	$(VERILATOR) -V

maintainer-copy::
clean mostlyclean maintainer-clean::
	-rm -rf obj_dir logs *.log *.dmp *.vpd coverage.dat core *.fst

dirs := source media header
src := source
files := $(foreach dir,$(dirs),$(wildcard $(dir)/*))
c_src := $(wildcard $(src)/*.c)
obj = $(c_src:.c=.o)
show:
	@echo $(files)
	@echo $(c_src)
	@echo $(obj)

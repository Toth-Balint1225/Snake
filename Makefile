#compiler flags
CXX := g++
CFLAGS := -Wall -Werror -std=c++17
GTKFLAGS := 

#directories 
bin_dir := bin
source_dir := src

#######manual input######################
target := Snake
#debug mode
CFLAGS += -g
#enable multithread
#CFLAGS += -pthread
#enable gtkmm
GTKFLAGS += `pkg-config gtkmm-3.0 --cflags --libs`
#########################################

#######do not edit below this line#######
cpps := $(shell ls $(source_dir)/*.cpp)
objs := $(patsubst $(source_dir)/%.cpp,$(bin_dir)/%.o,$(cpps))
deps := $(patsubst $(bin_dir)/%.o,$(bin_dir)/%.d,$(objs)) 

-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

all: $(target)
	@echo 'Running project $(target)'
	@echo '------------------'
	./$(target)
	@echo '------------------'
	@echo 'Project terminated'

$(target): $(objs) 
	@echo 'Invoking GNU linker'
	@echo 'Linking project $@'
	$(CXX) $(CFLAGS)  -o $@ $^ $(GTKFLAGS)
	@echo 'Linking finished'

$(bin_dir)/%.o: $(source_dir)/%.cpp
	@echo 'Invoking GNU compiler'
	@echo 'Compiling $<'
	$(CXX) $(CFLAGS) -c $< $(DEPFLAGS) -o $@ $(GTKFLAGS)

clean:
	rm -rf $(objs) $(deps) $(target)

.PHONY: $(target)


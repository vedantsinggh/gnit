TARGET_DIR := ~/bin
INCLUDE := include/
SRC := src/sha256.cpp src/cmd.cpp src/files.cpp src/log.cpp
ARGS := -Wall 
.SILENT:

build: main.cpp $(SRC) $(INCLUDE)
	g++ -I$(INCLUDE) $(SRC) $(ARGS) main.cpp -o gnit

install: $(SRC)
	g++ -I$(INCLUDE) $(SRC) main.cpp -o gnit
	@if [ ! -d $(TARGET_DIR) ]; then \
		mkdir $(TARGET_DIR); \
		echo '[INFO] creating $(TARGET_DIR)'; \
		echo export PATH='/home/$$USER/bin:$$PATH' >> ~/.bashrc; \
		source ~/.bashrc; \
	else \
		echo "[INFO] $(TARGET_DIR) folder already exists."; \
	fi
	cp gnit $(TARGET_DIR)
	echo "[INFO] added GNIT to .bashrc"

reset: .gnitconfig
	rm .gnitconfig
	rm .gnitstore

clean:
	rm -r ~/bin


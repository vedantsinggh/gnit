TARGET_DIR := ~/bin
INCLUDE := include/
SRC := src/sha256.cpp src/cmd.cpp src/files.cpp src/log.cpp

.SILENT:
compile: main.cpp $(SRC)
	g++ -I$(INCLUDE) $(SRC) main.cpp -o gnit

source: $(SRC)
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


clean:
	rm -r ~/bin


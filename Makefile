TARGET_DIR := ~/bin
.SILENT:
compile: main.cpp
	g++ -o gnit main.cpp 

source: main.cpp
	g++ -o gnit main.cpp
	@if [ ! -d $(TARGET_DIR) ]; then \
		mkdir $(TARGET_DIR); \
		echo "[INFO] creating $(TARGET_DIR)"; \
	else \
		echo "[INFO] $(TARGET_DIR) folder already exists."; \
	fi
	cp gnit $(TARGET_DIR)
	echo "[INFO] added GNIT to .bashrc"

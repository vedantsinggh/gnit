TARGET_DIR := ~/bin
SOURCE_FILES := main.cpp ./libs/sha256.cpp

.SILENT:
compile: $(SOURCE_FILES)
	g++ -o gnit $(SOURCE_FILES)

source: $(SOURCE_FILES)
	g++ -o gnit $(SOURCE_FILES)
	@if [ ! -d $(TARGET_DIR) ]; then \
		mkdir $(TARGET_DIR); \
		echo '[INFO] creating $(TARGET_DIR)'; \
		echo export PATH='/home/$$USER/bin:$$PATH' >> ~/.bashrc; \
	else \
		echo "[INFO] $(TARGET_DIR) folder already exists."; \
	fi
	cp gnit $(TARGET_DIR)
	echo "[INFO] added GNIT to .bashrc"


clean:
	rm -r ~/bin


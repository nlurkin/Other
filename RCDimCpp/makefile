all: 
	make -C xmlproxy
	make -C CppServer
	make -C TestClient
	make -C CServer
	
# Other Targets
clean:
	rm -rf obj
	rm -rf receivedfile.xml
	make -C CppServer clean
	make -C TestClient clean
	make -C CServer clean

.PHONY: all clean dependents
.SECONDARY:

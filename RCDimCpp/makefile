all: 
	- make -C xmlproxy
	make -C xmlproxy
	make -C CppServer
	make -C TestClient
	make -C CServer
	@mkdir -p Configuration
	@mkdir -p Configuration/Config
	@mkdir -p Configuration/Config/SubSystem
	@mkdir -p Configuration/Config/SubSystem/dimExample
	@mkdir -p Configuration/Report
	@mkdir -p Configuration/Report/SubSystem
	@mkdir -p Configuration/Report/SubSystem/dimExample
	 
	
# Other Targets
clean:
	rm -rf obj
	rm -rf receivedfile.xml
	make -C CppServer clean
	make -C TestClient clean
	make -C CServer clean
	make -C xmlproxy clean

.PHONY: all clean dependents
.SECONDARY:

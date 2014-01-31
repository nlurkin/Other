#include "dis.h"

void main_loop(){

}

int main(int argc, char** argv){
	char dimServerName = "dimExample";

	dis_start_serving(dimServerName);
	while(1){
		main_loop();
	}
}

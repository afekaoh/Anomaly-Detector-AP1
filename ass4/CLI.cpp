


#include "Command/CliCommand.h"
#include "IO/StandartIO.h"

int main() {
	
	CliCommand c("", StandartIO());
	while (!c.shouldStop()) {
		c.execute();
	}
}
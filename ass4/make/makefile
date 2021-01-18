a.out: CLI.o
	g++ -o CLI_AnomalyDtection CLI.o command.a io.a

CLI.o: ../CLI.cpp command.a io.a
	g++ -c -o CLI.o ../CLI.cpp

command.a: printCommand.o cliCommend.o uploadCommand.o downloadCommand.o ExitCommand.o
	ar -rv command.a CliCommand.o printCommand.o uploadCommand.o downloadCommand.o ExitCommand.O

cliCommend.o: io.a printCommand.o uploadCommand.o downloadCommand.o ExitCommand.o
	g++ -c ../Command/printCommand.cpp

printCommand.o: ../Command/printCommand.cpp ../Command/printCommand.h
	g++ -c ../Command/printCommand.cpp

uploadCommand.o: ../Command/uploadCommand.cpp ../Command/uploadCommand.h
	g++ -c ../Command/uploadCommand.cpp

downloadCommand.o: ../Command/downloadCommand.cpp ../Command/downloadCommand.h
	g++ -c ../Command/downloadCommand.cpp

ExitCommand.o: ../Command/ExitCommand.cpp ../Command/ExitCommand.h
	g++ -c ../Command/ExitCommand.cpp

io.a: ../IO/DefaultIO.h ../IO/StandartIO.cpp ../IO/StandartIO.h
	g++ -c ../IO/StandartIO.cpp
	ar -rv io.a StandartIO.o

run: a.out
	./CLI_AnomalyDtection
clean:
	rm -f *.o a.out

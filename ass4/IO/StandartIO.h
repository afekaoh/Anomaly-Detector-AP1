//
// Created by afeka on 17/11/2020.
//

#ifndef ADVANCEPROG1_STANDARTIO_H
#define ADVANCEPROG1_STANDARTIO_H


#include "DefaultIO.h"

class StandartIO : public DefaultIO {

public:
	virtual std::string read() const;
	
	virtual void write(std::string const &string) const;
	
	void upload(std::string const &fileLoc) const override;
	
	void download(std::string const &saveLocation) const override;
	
	void pressToContinue() const override;
};


std::string StandartIO::read() const {
	std::string string;
	std::getline(std::cin, string);
	return string;
}

void StandartIO::write(std::string const &string) const {
	std::cout << string;
}


void StandartIO::upload(std::ifstream data) const {
	if (!data.is_open()) {
		throw std::ios_base::failure(fileLoc);
	}
	char buffer[BUFFER_SIZE];
	if (data.good()) {
		std::ofstream toServer("./");
		while (!data.eof()) {
			data.read(buffer, BUFFER_SIZE);
			if (data.gcount() == 0) {
				throw std::ios_base::failure(fileLoc);
			}
			toServer.write(buffer, data.gcount());
		}
		toServer.close();
	}
	data.close();
}


void StandartIO::download(std::string const &saveLocation) const {
	auto save = saveLocation + "/file.csv";
	std::ofstream data(save);
	auto from = SERVER_LOC + "/file.csv";
	std::ifstream fromServer(from);
	
	if (!data.is_open()) {
		throw std::ios_base::failure(save);
	}
	if (!fromServer.is_open()) {
		throw std::ios_base::failure(from);
	}
	char buffer[BUFFER_SIZE];
	if (data.good() && fromServer.good()) {
		while (!fromServer.eof()) {
			fromServer.read(buffer, BUFFER_SIZE);
			if (fromServer.gcount() == 0) {
				throw std::ios_base::failure(from);
			}
			data.write(buffer, fromServer.gcount());
		}
	}
	fromServer.close();
	data.close();
	
}

void StandartIO::pressToContinue() const {
	std::cout << "press enter to continue...";
	std::cin.ignore();
	
}


#endif //ADVANCEPROG1_STANDARTIO_H

//
// Created by afeka on 17/11/2020.
//

#include <iostream>
#include <fstream>
#include "StandartIO.h"

static int const BUFFER_SIZE = 256;

static std::string const CLIENT_LOC = "/mnt/c/Users/afeka/OneDrive - Bar-Ilan University/Code projects/Advance-Programming/ass4/client";
static std::string const CSV_FILE = "/mnt/c/Users/afeka/OneDrive - Bar-Ilan University/Code projects/Advance-Programming/cmake-build-wsl_profile/trainFile1.csv";

static std::string const SERVER_LOC = "/mnt/c/Users/afeka/OneDrive - Bar-Ilan University/Code projects/Advance-Programming/ass4/server";


std::string StandartIO::read() const {
	std::string string;
	std::getline(std::cin, string);
	return string;
}

void StandartIO::write(std::string const &string) const {
	std::cout << string;
}


void StandartIO::upload(std::string const &fileLoc) const {
	std::ifstream data(fileLoc);
	
	if (!data.is_open()) {
		throw std::ios_base::failure(fileLoc);
	}
	char buffer[BUFFER_SIZE];
	if (data.good()) {
		std::ofstream toServer(SERVER_LOC);
		while (!data.eof()) {
			data.read(buffer, BUFFER_SIZE);
			if (data.gcount() == 0) {
				throw std::ios_base::failure(fileLoc);
			}
			toServer.write(buffer, data.gcount());
//			std::memset(buffer, 0, BUFFER_SIZE);
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

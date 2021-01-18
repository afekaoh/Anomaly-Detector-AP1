//
// Created by afeka on 17/11/2020.
//

#ifndef ADVANCEPROG1_DEFAULTIO_H
#define ADVANCEPROG1_DEFAULTIO_H


#include <string>

class DefaultIO {

public:
	virtual std::string read() const = 0;
	
	virtual void write(const std::string &) const = 0;
	
	virtual void upload(const std::string &) const = 0;
	
	virtual void download(const std::string &) const = 0;
	
	virtual void pressToContinue() const = 0;
	
};


#endif //ADVANCEPROG1_DEFAULTIO_H

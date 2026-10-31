
#include "etl/array.h"
#include "etl/string.h"
#include "etl/map.h"

#define FERRARI_CODE 	0x00
#define FIAT_CODE		0x01
#define FORD_CODE 		0x02
#define FAW_CODE		0x03

class CarModelCodeStorage {

	etl::map<uint8_t, std::string, 8> _brands;

public:

	CarModelCodeStorage() {
		this -> Init();
	}

	~CarModelCodeStorage(){}

	std::string GetBrand(uint8_t code) {
		auto it = _brands.find(code);
		if (it != _brands.end()) {
    		return it->second;
		}
	}

private:

	void Init() {
		_brands = {
    		etl::pair <uint8_t, std::string> (FERRARI_CODE, "Ferrari"),
    		etl::pair <uint8_t, std::string> (FIAT_CODE, "Fiat"),
    		etl::pair <uint8_t, std::string> (FORD_CODE, "Ford"),
    		etl::pair <uint8_t, std::string> (FAW_CODE, "FAW"),
		};
	}
	
};
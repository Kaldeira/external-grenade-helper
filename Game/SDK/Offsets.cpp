#include "Offsets.h"


/*
this gets offsets from a json file
you can generate one using https://github.com/a2x/cs2-dumper
at your own risk
*/

bool COffsets::ReadOffsets()
{
	std::ifstream file( "client_dll.json" );
	if ( !file.is_open() ) {
		std::cerr << "Erro ao abrir o arquivo client_dll.json\n";
		return false;
	}

	json j;
	file >> j;

	const auto& classes = j[ "client.dll" ][ "classes" ];
	for ( const auto& [className, classData] : classes.items() ) {
		const auto& fields = classData[ "fields" ];
		for ( const auto& [fieldName, offsetValue] : fields.items() ) {
			clientOffsets[ className ][ fieldName ] = offsetValue.get<int>();
		}
	}

	j.clear();
	file.close();

	file.open( "offsets.json" );
	if ( !file.is_open() ) {
		std::cerr << "Erro ao abrir o arquivo offsets.json\n";
		return false;
	}

	file >> j;

	for ( const auto& [modulo, dados] : j.items() ) {
		for ( const auto& [nomeOffset, valorOffset] : dados.items() ) {
			baseOffsets[ modulo ][ nomeOffset ] = valorOffset.get<uint32_t>();
		}
	}

	//printf( "entityList: %llx\n", baseOffsets[ "client.dll" ][ "dwEntityList" ] );

	return true;
}
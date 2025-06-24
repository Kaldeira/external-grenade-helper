#pragma once

#include "../Includes.h"

#pragma once

struct GrenadeData
{
	std::string MapName;
	std::string Name;
	std::string Desc;
	std::string Nade;
	Vector3 Pos;
	Vector2 Angle;
};

class CNadeHelper : public CSingleton<CNadeHelper>
{
public:
	CNadeHelper();
	void DrawNadeHelper();
	void HandlerNadeHelper();
	void DrawRecoilHelper();

	bool SaveCurrentNade(std::string nadeName, std::string nadeDesc);

	void SaveIntoFile(GrenadeData grenade);

	std::vector<GrenadeData> ReadNadeFile();

	bool DeleteFromFile(std::string nadeName);

private:
	class CRender* pRender = nullptr;
	class CSDK* pSDK = nullptr;
	class CEntity* pEntity = nullptr;
	class CGlobals* pGlobals = nullptr;

	std::vector<GrenadeData> vecNade = {};
	json jsonNadeHelper;
	std::string nadeFileName = "nades_lineup.json";
	bool updateVector = true;

	void Draw3DCircle( Vector3 position, float points, float radius, struct ImVec4 color );
	void AngleVectors( const Vector2& angles, Vector3* forward );
	bool isCloseEnough( const Vector3& a, const Vector3& b, float tolerance = 0.01f );

	bool check_file()
	{
		std::ifstream inputFile( nadeFileName.c_str() );
		json loadedJson;
		if ( inputFile.is_open() ) {
			inputFile >> loadedJson;
			inputFile.close();

			return true;
		}
		else {
			std::cerr << "Erro ao abrir o arquivo JSON." << std::endl;
			return false;
		}

		return false;
	}

	void removeElement( json& jsonData, const std::string& nameToRemove ) {
		jsonData.erase( std::remove_if( jsonData.begin(), jsonData.end(),
			[ nameToRemove ]( const json& element ){
				return element[ "Name" ] == nameToRemove;
			} ),
			jsonData.end() );
	}
};

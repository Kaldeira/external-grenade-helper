#pragma once
#include "../Includes.h"

class CLocalData
{
public:
	uint64_t m_dwEntity;
	uint64_t m_dwController;
	uint64_t m_dwActiveWeapon;
	uint64_t m_dwCamera;

	int m_iTeamNum;
	int m_iItemIndex;
	int m_iShotsFired;
	int m_iFOVStart;
	int m_iDefaultFov;

	float m_fMouseSens;

	Vector2 m_vecAngles;
	Vector3 m_vecAimPunch;
	Vector3 m_vecOrigin;
	Vector3 m_vecVelocity;
	Vector3 m_vecEyePos;

	D3DMATRIX m_mViewMatrix;
};

class CEntity : public CSingleton<CEntity>
{
public:
	CEntity();
	void Run();
	bool UpdatePointers();
	bool UpdateLocalPlayer();

	CLocalData localPlayer;

private:
	class CMemory* pMemory = nullptr;
	class CGlobals* pGlobals = nullptr;
	class CSDK* pSDK = nullptr;
	class COffsets * pOffset = nullptr;
};
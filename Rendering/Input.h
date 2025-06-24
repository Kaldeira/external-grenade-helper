#pragma once
#include "../Includes.h"

class CInput : public CSingleton<CInput>
{
public:

	bool Click( int k );

	bool Hold( int k );

	bool Release( int k );

	void UpdateKeyStates();

private:
	std::array<bool, 256> key_state;
	std::array<bool, 256> prev_state;
	std::array<bool, 256> key_down;
	bool updated = false;

	class CRenderer* pRender = nullptr;
};

#define m_pInput CInput::GetInstance()
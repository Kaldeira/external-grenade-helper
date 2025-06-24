
#include "input.h"

bool CInput::Click( int k )
{
	return key_state.at( k ) && !prev_state.at( k );
}

bool CInput::Hold( int k )
{
	return key_state.at( k );
}

bool CInput::Release( int k )
{
	return !key_state.at( k ) && prev_state.at( k );
}

void CInput::UpdateKeyStates()
{
	std::copy( key_state.begin(), key_state.end(), prev_state.begin() );
	for ( int i = VK_LBUTTON; i <= VK_RMENU; i++ )
		key_state.at( i ) = GetAsyncKeyState( i );
}

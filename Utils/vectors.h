#pragma once

class Vector3
{
public:
	inline Vector3() {
		x = y = z = 0.0f;
	}

	static Vector3 Zero() {
		return Vector3( 0.0f, 0.0f, 0.0f );
	}

	inline Vector3( float X, float Y, float Z ) {
		x = X; y = Y; z = Z;
	}

	inline Vector3( float* v )
	{
		x = v[ 0 ]; y = v[ 1 ]; z = v[ 2 ];
	}

	inline Vector3( const float* v )
	{
		x = v[ 0 ]; y = v[ 1 ]; z = v[ 2 ];
	}

	inline float& operator[]( int i )
	{
		return ( (float*) this )[ i ];
	}

	inline float operator[]( int i ) const
	{
		return ( (float*) this )[ i ];
	}

	inline Vector3& operator-=( float v ) {
		x -= v; y -= v; z -= v; return *this;
	}

	inline Vector3& operator+=( float v ) {
		x += v; y += v; z += v; return *this;
	}

	inline Vector3 operator*( float v ) const {
		return Vector3( x * v, y * v, z * v );
	}

	inline Vector3 operator/( float v ) const
	{
		return Vector3( x / v, y / v, z / v );
	}

	inline Vector3& operator-=( const Vector3& v ) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	inline Vector3& operator+=( const Vector3& v ) {
		x += v.x; y += v.y; z += v.z; return *this;
	}

	Vector3 operator - () const {
		return Vector3( -x, -y, -z );
	}

	inline Vector3 operator-( const Vector3& v ) const {
		return Vector3( x - v.x, y - v.y, z - v.z );
	}

	inline Vector3 operator+( const Vector3& v ) const {
		return Vector3( x + v.x, y + v.y, z + v.z );
	}

	inline Vector3 operator*( const Vector3& v ) const {
		return Vector3( x * v.x, y * v.y, z * v.z );
	}

	inline bool operator==( const Vector3& v ) {
		return this->x == v.x && this->y == v.y && this->z == v.z;
	}

	inline Vector3& operator*=( float fl )
	{
		x *= fl;
		y *= fl;
		z *= fl;

		return *this;
	}

	inline Vector3& operator/=( float v )
	{
		x /= v; y /= v; z /= v; return *this;
	}

	inline Vector3 operator- ( float f ) const
	{
		return Vector3( x - f, y - f, z - f );
	}

	inline Vector3 operator+ ( float f ) const
	{
		return Vector3( x + f, y + f, z + f );
	}
	inline bool operator!=( Vector3 v )
	{
		if ( x != v.x || y != v.y || z != v.z ) return true;

		return false;
	}

	inline bool operator!( void ) const
	{
		return IsZero();
	}

	inline bool Vector3::IsZero( void ) const
	{
		return ( x == 0.0f && y == 0.0f && z == 0.0f );
	}

	bool Vector3::IsValid( void ) const
	{
		return !IsZero();
	}

	bool Vector3::IsEncrypted( void )
	{
		if ( ( x > -100000.f && x < 100000.f ) && ( y > -100000.f && y < 100000.f ) && ( z > -100000.f && z < 100000.f ) )
			return false;

		return true;
	}

	inline Vector3& Clear( void )
	{
		x = y = z = 0;

		return *this;
	}

	inline float Sum()
	{
		return x + y + z;
	}

	inline float Dot( const Vector3& other )
	{
		return x * other.x + y * other.y + z * other.z;
	}

	inline float Length( void )
	{
		float Length = 0;

		Length = sqrt( LengthSqr() );

		return Length;
	}

	inline float LengthSqr( void )
	{
		return x * x + y * y + z * z;
	}

	inline float magnitude()
	{
		return sqrt( x * x + y * y + z * z );
		//return (float)sqrt(x * x + z * z);
	}

	inline float Distance( Vector3& ToVector )
	{
		return ( ToVector - *this ).Length();
	}

	inline float Distance( const Vector3& ToVector ) const
	{
		return ( ToVector - *this ).Length();
	}
	float x, y, z;
};

class Vector2
{
public:
	inline Vector2() {
		x = y = 0.0f;
	}

	static Vector2 Zero() {
		return Vector2( 0.0f, 0.0f );
	}

	inline Vector2( float X, float Y ) {
		x = X; y = Y;
	}

	inline float operator[]( int i ) const {
		return ( (float*) this )[ i ];
	}

	inline Vector2& operator-=( float v ) {
		x -= v; y -= v; return *this;
	}

	inline Vector2 operator*( float v ) const {
		return Vector2( x * v, y * v );
	}

	inline Vector2 operator/( float v ) const
	{
		return Vector2( x / v, y / v );
	}

	inline Vector2& operator-=( const Vector2& v ) {
		x -= v.x; y -= v.y; return *this;
	}

	inline Vector2& operator+=( const Vector2& v ) {
		x += v.x; y += v.y; return *this;
	}

	inline Vector2 operator-( const Vector2& v ) const {
		return Vector2( x - v.x, y - v.y );
	}

	inline Vector2 operator+( const Vector2& v ) const {
		return Vector2( x + v.x, y + v.y );
	}

	inline bool IsZero( void ) const
	{
		return x == 0.0f && y == 0.0f;
	}

	bool IsValid( void )
	{
		return ( x != 0 && y != 0 );
	}

	inline Vector2& Clear( void )
	{
		x = y = 0;

		return *this;
	}

	inline float Dot( const Vector2& other )
	{
		return x * other.x + y * other.y;
	}

	inline float Length( void )
	{
		float Length = 0;

		Length = sqrt( LengthSqr() );

		return Length;
	}

	inline float LengthInverted( void )
	{
		float Length = 0;

		Length = sqrt( LengthInvSqr() );

		return Length;
	}

	inline float LengthSqr( void )
	{
		return x * x + y * y;
	}

	inline float LengthInvSqr( void )
	{
		return y * y + x * x;
	}

	inline float Distance( Vector2& ToVector )
	{
		return ( ToVector - *this ).Length();
	}

	float x, y;
};

#define M_PI     3.14159265358979323846
#define M_PI_2   6.28318530717958647692
#define D3DX_PI    ((FLOAT)  3.1415927f)

#define VECTOR2_ZERO Vector2(0, 0);
#define VECTOR3_ZERO Vector3(0, 0, 0);
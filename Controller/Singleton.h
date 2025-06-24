#pragma once
template<typename t>
class CSingleton {
public:
	template<typename... Args>
	static t* GetInstance( Args&&... args );
protected:
	CSingleton() = default;
	~CSingleton() = default;
};

template<typename t>
template<typename... Args>
t* CSingleton<t>::GetInstance( Args&&... args )
{
	static t GetInstance( std::forward<Args>( args )... );
	return &GetInstance;
}

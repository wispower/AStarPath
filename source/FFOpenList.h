#pragma once

#include <vector>

template< class _Ty >
class FFOpenList
{
public:
	FFOpenList(void)
		: m_container()
	{

	}
	~FFOpenList(void) {}

	void push( _Ty& _Val )
	{
		if( m_container.size() == 0 ){
			m_container.push_back( _Val );
		}
		else{
			int nSize = m_container.size();
			int nInsertPos = nSize;
			for( int i = 0; i < nSize; i++ ){
				if( _Val < m_container[i] ){
					nInsertPos = i;
					break;
				}
			}
			if( nInsertPos == nSize ){
				m_container.push_back( _Val );
			}
			else{
				m_container.push_back( m_container[nSize-1] );
				for( int i = nSize - 1; i > nInsertPos; i-- ){
					m_container[i] = m_container[i-1];
				}
				m_container[nInsertPos] = _Val;
			}
		}
	}

	_Ty& top()
	{
		return m_container.front();
	}

	void pop()
	{
		int nSize = m_container.size();
		int nInsertPos = nSize;
		for( int i = 0; i < nSize-1; i++ ){
			m_container[i] = m_container[i+1];
		}
		m_container.pop_back();
	}

	bool empty()
	{
		return ( m_container.size() == 0 );
	}

	bool refresh( _Ty& _Val )
	{
		int nsize = m_container.size();
		int refreshIndex = -1;
		for( int i = 0; i < nsize; i++ ){
			if( m_container[i] == _Val ){
				refreshIndex = i;
				break;
			}
		}
		if( refreshIndex == -1 ){
			return false;
		}

		if( _Val < m_container[refreshIndex] ){

			int nInsertPos = refreshIndex;
			for( int i = 0; i < refreshIndex; i++ ){
				if( _Val < m_container[i] ){
					nInsertPos = i;
					break;
				}			
			}
			for( int i = refreshIndex; i > nInsertPos; i-- ){
				m_container[i] = m_container[i-1];
			}
			m_container[nInsertPos] = _Val;
		}
		else{
			return false;
		}

		return true;
	}

private:
	std::vector< _Ty > m_container;
};


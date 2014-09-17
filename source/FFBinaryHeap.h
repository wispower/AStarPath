#pragma once

#include <vector>

template< class _Ty >
class FFBinaryHeap
{
public:
	FFBinaryHeap(void): m_container()
	{
		_Ty t( 0, 0, 0, 0, 0);
		m_container.push_back( t );
	}
	~FFBinaryHeap(void){}

	void push( _Ty& _Val )
	{
		m_container.push_back( _Val );
		int nIndex = m_container.size() - 1;

		if( nIndex == 9 ){
			int i = 0;
		}
		if( nIndex > 1 ){
			int nParentIndex = nIndex / 2;
			while( nIndex != 1 ){
				if( m_container[ nIndex ] < m_container[ nParentIndex ] ){
					_Ty temp = m_container[ nParentIndex ];
					m_container[ nParentIndex ] = m_container[ nIndex ];
					m_container[ nIndex ] = temp;
					nIndex = nParentIndex;
					nParentIndex = nIndex / 2;
				}
				else{
					break;
				}
			}
		}
	}

	_Ty& top()
	{
		return m_container[1];
	}

	void pop()
	{
		int nSize = m_container.size();
		if( nSize > 2 ){
			m_container[ 1 ] = m_container[ nSize - 1 ];
			nSize -= 1;
			int nIndex = 1;
			int nLeftChild = nIndex * 2;
			int nRightChild = nLeftChild + 1;
			int nNext;
			bool bStoped;
			while( nLeftChild < nSize ){
				bStoped = true;
				if( nRightChild >= nSize ||
					m_container[ nLeftChild ] < m_container[ nRightChild ] ){

					if( m_container[ nLeftChild ] < m_container[ nIndex ] ){
						_Ty temp = m_container[ nLeftChild ];
						m_container[ nLeftChild ] = m_container[ nIndex ];
						m_container[ nIndex ] = temp;
						nNext = nLeftChild;
						bStoped = false;
					}
				}
				else{
					_Ty temp = m_container[ nRightChild ];
					m_container[ nRightChild ] = m_container[ nIndex ];
					m_container[ nIndex ] = temp;
					nNext = nRightChild;
					bStoped = false;
				}

				if( bStoped == false ){
					nIndex = nNext;
					nLeftChild = nIndex * 2;
					nRightChild = nLeftChild + 1;
				}
				else{
					break;
				}
			}
		}
		m_container.pop_back();
	}

	bool empty()
	{
		return ( m_container.size() == 1 );
	}

	bool refresh( _Ty _Val )
	{
		int nsize = m_container.size();
		int nRefreshIndex = -1;
		for( int i = 1; i < nsize; i++ ){
			if( m_container[i] == _Val ){
				nRefreshIndex = i;
				break;
			}
		}
		if( nRefreshIndex == -1 ){
			return false;
		}

		if( _Val < m_container[nRefreshIndex] ){
			m_container[nRefreshIndex] = _Val;
			int nParentIndex = nRefreshIndex / 2;
			while( nRefreshIndex != 1 ){
				if( m_container[ nRefreshIndex ] < m_container[ nParentIndex ] ){
					_Ty temp = m_container[ nParentIndex ];
					m_container[ nParentIndex ] = m_container[ nRefreshIndex ];
					m_container[ nRefreshIndex ] = temp;
					nRefreshIndex = nParentIndex;
					nParentIndex = nRefreshIndex / 2;
				}
				else{
					break;
				}
			}
		}
		else{
			return false;
		}

		return true;
	}
private:
	std::vector< _Ty > m_container;
};


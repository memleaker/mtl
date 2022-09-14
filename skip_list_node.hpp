#ifndef MTL_SKIP_LIST_NODE_H
#define MTL_SKIP_LIST_NODE_H

#include <ctime>
#include <cstdio>
#include <cstdlib>

namespace mtl {

template <typename Score, typename Tp>
class skip_list_node {
public:
	skip_list_node()
	{
		try {
            // set m_level first, because init_forward_ptr will use it
			this->m_level = this->m_max_level;
			this->init_forward_ptr();
		}
		catch (...) {
			throw;
		}
	}

	explicit skip_list_node(const Score& score, const Tp& data)
	{
		try {
			// set m_level first, because init_forward_ptr will use it
			this->m_level = this->random_level();
            this->init_forward_ptr();
			this->m_data = data;
			this->m_score = score;
		}
		catch (...) {
			throw;
		}
	}
	
	~skip_list_node() noexcept
	{
		::operator delete(this->m_forwards);
	}

public:
	inline void set_forward(int level, mtl::skip_list_node<Score, Tp> *forward)
	{
		if (level == 0 || level > this->m_level)
			return;

		this->m_forwards[level-1] = forward;
	}

	inline mtl::skip_list_node<Score, Tp>* get_forward(int level) const noexcept
	{
		if (level == 0 || level > this->m_level)
			return nullptr;

		return this->m_forwards[level-1];
	}

	inline const Tp& first(void) const noexcept
	{
		return this->m_score;
	}

	inline const Score& second(void) const noexcept
	{
		return this->m_data;
	}

	inline int get_level(void) const noexcept
	{
		return this->m_level;
	}

	inline bool greater_than(const Score& score) const noexcept
	{
		return this->m_score > score;
	}

	inline bool lesser_than(const Score& score) const noexcept
	{
		return this->m_score < score;
	}

private:
    void init_forward_ptr(void)
    {
        try {
            this->m_forwards = static_cast<skip_list_node**>(::operator new(
                                this->m_level * sizeof(skip_list_node*)));

            for (int i = 0; i < this->m_level; ++i)
				this->m_forwards[i] = nullptr;
        }
        catch(...) {
            if (this->m_forwards != nullptr) {
				::operator delete(this->m_forwards);
			}
			throw;
        }
    }

	inline int random_level(void)
	{
		int level = 1;

		while ((::std::rand() & 0x1)) level++;

		return (level > this->m_max_level) ? this->m_max_level : level;
	}

private:
	Score m_score;
	Tp m_data;
	int m_level;
	skip_list_node<Score, Tp>** m_forwards;

	const static int m_max_level = 32;
};  // end of class skip_list_node

} // end of namespace mtl

#endif
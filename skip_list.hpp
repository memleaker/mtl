#ifndef MTL_SKIP_LIST_H
#define MTL_SKIP_LIST_H

#include <cstdio>

#include "skip_list_node.hpp"

namespace mtl {

template <typename Score, typename Tp>
class skip_list {
	// Constructor andp Destructor.
public:
	skip_list() noexcept
	{
		this->m_len = 0;
		this->m_level = 1;
	}

	~skip_list() noexcept
	{
		skip_list_node<Score, Tp> *temp;
		skip_list_node<Score, Tp> *walk = this->m_head.get_forward(1);

		while (walk)
		{
			temp = walk->get_forward(1);
			walk->~skip_list_node();
			walk = temp;
		}
	}
	
	// type.
public:
	using value_type = Tp;
	using size_type = ::std::size_t;
	using score_reference = Score&;
	using val_reference = Tp&;
	using const_score_reference = const Score&;
	using const_val_reference = const Tp&;

	class iterator {
	public:
		iterator(mtl::skip_list_node<Score, Tp> *ptr) : ptr(ptr) {}
		
		void operator++()
		{
			if (this->ptr) {
				this->ptr = this->ptr->get_forward(1);
			}
		}

		void operator++(int)
		{
			if (this->ptr) {
				this->ptr = this->ptr->get_forward(1);
			}
		}

		bool operator!=(const iterator &it)
		{
			return this->ptr != it.ptr;
		}

		mtl::skip_list_node<Score, Tp> operator*()
		{
			return *this->ptr;
		}

		mtl::skip_list_node<Score, Tp>* operator->()
		{
			return this->ptr;
		}

	private:
		mtl::skip_list_node<Score, Tp> *ptr;
	};

	using const_iterator = const iterator;

public:
	void insert(const_score_reference score, const_val_reference data)
	{
		mtl::skip_list_node<Score, Tp> *node {nullptr};
		mtl::skip_list_node<Score, Tp> *walk {nullptr};
		mtl::skip_list_node<Score, Tp> *forward {nullptr};

		try {
			node = new mtl::skip_list_node<Score, Tp>(score, data);

			for (int idx = node->get_level(); \
					idx > 0; idx--)
			{
				// find position
				walk = &this->m_head;
				while ((forward = walk->get_forward(idx)) &&
						(node->greater_than(forward->first())))
					walk = forward;

				// insert
				node->set_forward(idx, forward);
				walk->set_forward(idx, node);
			}

			if (this->m_level < node->get_level())
				this->m_level = node->get_level();

			this->m_len++;
		}
		catch (...) {
			if (node != nullptr) {
				delete node;
			}
			throw;
		}
	}
	
	void erase(const_score_reference score)
	{
		mtl::skip_list_node<Score, Tp> *walk {nullptr};
		mtl::skip_list_node<Score, Tp> *target {nullptr};
		mtl::skip_list_node<Score, Tp> *backward {nullptr};

		for (int idx = this->m_level; \
				idx > 0; idx--)
		{
			// find position
			backward = &this->m_head;
			while ((walk = backward->get_forward(idx)) &&
				   (walk->lesser_than(score)))
			{
				backward = walk;
			}

			// walk is the last node or walk->score > score
			if (walk == nullptr || walk->greater_than(score)) 
				continue;

			// walk->score is equal to score.
			target = walk;
			backward->set_forward(idx, walk->get_forward(idx));

			if (walk->get_forward(idx) == nullptr) {
				this->m_level--;
			}
		}

		if (target) {
			target->~skip_list_node();
			this->m_len--;
		}
	}

	const_iterator find(const_score_reference score) const noexcept
	{
		mtl::skip_list_node<Score, Tp> *walk {nullptr};

		for (int idx = this->m_level; \
				idx > 0; idx--)
		{
			// find position
			for (walk = this->m_head.get_forward(idx); walk; \
					walk = walk->get_forward(idx))
			{
				// walk->score >= score
				if (!walk->lesser_than(score)) {
					break;
				}
			}

			// walk is the last node or walk->score > score
			if (walk == nullptr || walk->greater_than(score)) 
				continue;

			// walk->score is equal to score.
			return walk;
		}

		return nullptr;
	}

	void clear(void) noexcept
	{
		int i;
		skip_list_node<Score, Tp> *temp;
		skip_list_node<Score, Tp> *walk = this->m_head.get_forward(1);

		while (walk)
		{
			temp = walk->get_forward(1);
			walk->~skip_list_node();
			walk = temp;
		}

		for (i = 1; i <= this->m_head.get_level(); i++)
		{
			this->m_head.set_forward(i, nullptr);
		}

		this->m_len = 0;
		this->m_level = 1;
	}

	inline bool empty(void) const noexcept
	{
		return this->m_len == 0;
	}

	inline int size(void) const noexcept
	{
		return this->m_len;
	}

	inline iterator begin(void) noexcept
	{
		return this->m_head.get_forward(1);
	}
	
	inline const_iterator cbegin(void) const noexcept
	{
		return this->m_head.get_forward(1);
	}
	
	inline iterator end(void) noexcept
	{
		return nullptr;
	}

	inline const_iterator cend(void) const noexcept
	{
		return nullptr;
	}

	void show(void) noexcept
	{
		mtl::skip_list_node<Score, Tp> *walk;

		for (int idx = this->m_level; \
				idx > 0; idx--)
		{
			std::cout << "[level " << idx << "] ";
			
			for (walk = this->m_head.get_forward(idx); walk; \
				 walk = walk->get_forward(idx))
				std::cout << walk->second() << "->";
			
			std::cout << "\n";
		}
	}

private:
	size_type m_len;
	int m_level;
	mtl::skip_list_node<Score, Tp> m_head;

}; // end of class skip_list

} // end of namespace mtl

#endif
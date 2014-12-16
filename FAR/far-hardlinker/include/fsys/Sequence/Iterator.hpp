#ifndef _FSYS_SEQUENCE_ITERATOR_HPP_
#define _FSYS_SEQUENCE_ITERATOR_HPP_

namespace fsys {

	///=================================================================================================================
	class Sequence::ci_iterator {
		typedef ci_iterator this_type;

	public:
		this_type& operator ++();

		this_type operator ++(int);

		const value_type& operator *() const;

		const value_type* operator ->() const;

		bool operator ==(const this_type& rhs) const;

		bool operator !=(const this_type& rhs) const;

	private:
		ci_iterator();

		ci_iterator(const Sequence& seq);

		struct impl;
		simstd::shared_ptr<impl> m_impl;

		friend class Sequence;
	};

	///=================================================================================================================
	struct Sequence::ci_iterator::impl {
		~impl() noexcept;
		impl() noexcept;
		impl(const Sequence& seq) noexcept;

		const Sequence* m_sequence;
		HANDLE          m_find_handle;
		FindStat        m_fstat;
	};

}

#endif

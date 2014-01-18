#ifndef LIBSTL_MEMORY_ALLOCATOR_TRAITS_HPP_
#define LIBSTL_MEMORY_ALLOCATOR_TRAITS_HPP_

namespace simstd {

	template<typename Allocator>
	struct allocator_traits {
		typedef Allocator                           allocator_type;
		typedef typename Allocator::value_type      value_type;
		typedef typename Allocator::size_type       size_type;
		typedef typename Allocator::difference_type difference_type;
		typedef typename Allocator::reference       reference;
		typedef typename Allocator::const_reference const_reference;
		typedef typename Allocator::pointer         pointer;
		typedef typename Allocator::const_pointer   const_pointer;
		typedef void*                               void_pointer;
		typedef const void*                         const_void_pointer;
	};

}

#endif

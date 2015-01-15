#ifndef BASIS_SYS_MEMORY_HEAP_STAT_HPP_
#define BASIS_SYS_MEMORY_HEAP_STAT_HPP_

namespace memory {
	namespace heap {

		struct AllocatedItem {
			AllocatedItem(void* ptr, uint64_t size, const char* function, int line);

			uint64_t    size;
			void*       ptr;
			const char* function;
			ssize_t     line;
		};

		struct StatNone
		{
			void*    commit_alloc(void* ptr, uint64_t size, const char* function = "", int line = 0) {UNUSED(size); UNUSED(function); UNUSED(line); return ptr;}
			void     commit_free(const void* ptr, uint64_t size, const char* function = "", int line = 0) {UNUSED(size); UNUSED(function); UNUSED(line); UNUSED(ptr);}

			uint64_t get_allocations() const {return uint64_t();}
			uint64_t get_frees() const {return uint64_t();}
			uint64_t get_allocations_size() const {return uint64_t();}
			uint64_t get_frees_size() const {return uint64_t();}

			void     get_database(AllocatedItem*& db, size_t& size) const;
			void     free_database(AllocatedItem* db) const;
		};

		struct StatCount
		{
			~StatCount();
			StatCount();

			void*    commit_alloc(void* ptr, uint64_t size, const char* function = "", int line = 0);
			void     commit_free(const void* ptr, uint64_t size, const char* function = "", int line = 0);

			uint64_t get_allocations() const {return allocations;}
			uint64_t get_frees() const {return frees;}
			uint64_t get_allocations_size() const {return allocSize;}
			uint64_t get_frees_size() const {return freeSize;}

			void     get_database(AllocatedItem*& db, size_t& size) const;
			void     free_database(AllocatedItem* db) const;

		private:
			uint64_t allocations;
			uint64_t frees;
			uint64_t allocSize;
			uint64_t freeSize;

			void*    database;
		};

		struct StatLog: public StatCount
		{
			void* commit_alloc(void* ptr, uint64_t size, const char* function = "", int line = 0);
			void  commit_free(const void* ptr, uint64_t size, const char* function = "", int line = 0);
		};

		template<typename HeapType, typename StatType>
		struct DecoratorStat: public HeapType
		{
			typedef StatType stat_type;

			static void* alloc(size_t size, const char* function = "", int line = 0);
			static void* realloc(void* ptr, size_t size, const char* function = "", int line = 0);
			static void  free(const void* ptr, const char* function = "", int line = 0);

			static const stat_type& get_stat();

		private:
			static stat_type& get_stat_impl();
		};

	}
}

namespace memory {
	namespace heap {

		template<typename HeapType, typename StatType>
		void* DecoratorStat<HeapType, StatType>::alloc(size_t size, const char* function, int line)
		{
			TraceFunc();
			return get_stat_impl().commit_alloc(HeapType::alloc(size, function, line), size, function, line);
		}

		template<typename HeapType, typename StatType>
		void* DecoratorStat<HeapType, StatType>::realloc(void* ptr, size_t size, const char* function, int line)
		{
			TraceFunc();
			get_stat_impl().commit_free(ptr, HeapType::size(ptr), function, line);
			return get_stat_impl().commit_alloc(HeapType::realloc(ptr, size, function, line), size, function, line);
		}

		template<typename HeapType, typename StatType>
		void DecoratorStat<HeapType, StatType>::free(const void* ptr, const char* function, int line)
		{
			TraceFunc();
			get_stat_impl().commit_free(ptr, HeapType::size(ptr), function, line);
			HeapType::free(ptr);
		}

		template<typename HeapType, typename StatType>
		const typename DecoratorStat<HeapType, StatType>::stat_type& DecoratorStat<HeapType, StatType>::get_stat()
		{
			TraceFunc();
			return get_stat_impl();
		}

		template<typename HeapType, typename StatType>
		typename DecoratorStat<HeapType, StatType>::stat_type& DecoratorStat<HeapType, StatType>::get_stat_impl()
		{
			TraceFunc();
			static stat_type stat;
			return stat;
		}

	}
}

#endif

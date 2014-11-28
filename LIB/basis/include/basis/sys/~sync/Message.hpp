#ifndef BASIS_SYS_SYNC_MESSAGE_HPP_
#define BASIS_SYS_SYNC_MESSAGE_HPP_

#include <basis/sys/sync.hpp>

namespace sync {

	struct Message {
		typedef ssize_t type_t;
		typedef ssize_t code_t;
		typedef ssize_t param_t;
		typedef void * data_t;

		static const type_t MASK_ALL_TYPES = ~static_cast<type_t>(0);
		static const code_t MASK_ALL_CODES = ~static_cast<code_t>(0);

		~Message();

		Message(const type_t & type = type_t(), const code_t & code = code_t(), const param_t & param = param_t(), const data_t & data = data_t());

		type_t get_type() const;

		code_t get_code() const;

		param_t get_param() const;

		data_t get_data() const;

		void set_type(const type_t & in);

		void set_code(const code_t & in);

		void set_param(const param_t & in);

		void set_data(const data_t & in);

	private:
		type_t  m_type;
		code_t  m_code;
		param_t m_param;
		data_t  m_data;
	};

}

#endif

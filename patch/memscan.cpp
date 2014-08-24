#include "memscan.h"

template<class T> uint32_t memscan<T>::search_byte(T val, T *begin, T *end, search_type type) {
	uint64_t found = 0;
	/* move value to our aligned member variable */
	for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
		_val[j] = val;
	}

	switch(type) {
		case search_equal:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_BYTE(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) == _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_gt:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_BYTE_GT(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) > _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_ge:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_BYTE_GT(i+j, _val-1, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) >= _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_lt:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_BYTE_GT(_val, i+j,found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) < _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_le:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_BYTE_GT(_val+1, i+j, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) <= _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		default:
			break;
	}

	return _vec_results.size();
}

template<class T> uint32_t memscan<T>::search_word(T val, T *begin, T *end, search_type type) {
	uint64_t found = 0;
	/* move value to our aligned member variable */
	for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
		_val[j] = val;
	}

	switch(type) {
		case search_equal:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_WORD(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) == _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_gt:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_WORD_GT(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) > _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_ge:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_WORD_GT(i+j, _val-1, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) >= _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_lt:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_WORD_GT(_val, i+j,found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) < _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_le:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_WORD_GT(_val+1, i+j, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) <= _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		default:
			break;
	}

	return _vec_results.size();
}

template<class T> uint32_t memscan<T>::search_dword(T val, T *begin, T *end, search_type type) {
	uint64_t found = 0;
	/* move value to our aligned member variable */
	for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
		_val[j] = val;
	}

	switch(type) {
		case search_equal:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_DWORD(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) == _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_gt:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_DWORD_GT(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) > _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_ge:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_DWORD_GT(i+j, _val-1, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) >= _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_lt:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_DWORD_GT(_val, i+j,found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) < _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_le:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_DWORD_GT(_val+1, i+j, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) <= _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		default:
			break;
	}

	return _vec_results.size();
}

template<class T> uint32_t memscan<T>::search_qword(T val, T *begin, T *end, search_type type) {
	uint64_t found = 0;
	/* move value to our aligned member variable */
	for(uint8_t j = 0; j < 16/sizeof(T); ++j) {
		_val[j] = val;
	}

	switch(type) {
		case search_equal:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_QWORD(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) == _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_gt:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_QWORD_GT(i+j, _val, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) > _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_ge:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_QWORD_GT(i+j, _val-1, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) >= _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_lt:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_QWORD_GT(_val, i+j,found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) < _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		case search_le:
			for(T *i = begin; i < end; i += 16) {
				/* check against every possible alignment */
				for(uint8_t j = 0; j < sizeof(T); ++j) {
					CHECK_QWORD_GT(_val+1, i+j, found);

					if( found != 0 ) {
						/* where exactly? */
						for(uint8_t k = 0; k < 17-sizeof(T); k += sizeof(T)) {
							if(*((T*) ((uint8_t*)i+j+k)) <= _val[0])
								_vec_results.push_back((T*) ((uint8_t*)i+j+k));
						}
					}
				}
			}
			break;
		default:
			break;
	}
	return _vec_results.size();
}
	

template<class T> uint32_t memscan<T>::search_cont(search_type type, T val) {
	typename std::vector<T*> vec;
	typename std::vector<T*>::iterator it;
	switch(type) {
		case search_equal:
			for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
				if( **it == val )
					vec.push_back(*it);
			}
			break;
		case search_notequal:
			for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
				if( **it != val )
					vec.push_back(*it);
			}
			break;
		case search_gt:
			for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
				if( **it > val )
					vec.push_back(*it);
			}
			break;
		case search_ge:
			for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
				if( **it >= val )
					vec.push_back(*it);
			}
			break;
		case search_lt:
			for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
				if( **it < val )
					vec.push_back(*it);
			}
			break;
		case search_le:
			for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
				if( **it <= val )
					vec.push_back(*it);
			}
			break;
		case search_unchanged:
			for(it = _vec_results.begin(); it < _vec_results.end(); ++it) {
				if( **it == _val )
					vec.push_back(*it);
			}
			break;
		default:
			break;
	}

	_vec_results = vec;
}

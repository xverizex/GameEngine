#ifndef GLOBAL_H
#define GLOBAL_H

namespace Global {

	template<typename T>
	static T* m;

	template<typename T>
	void set_singleton (T* sm) {
		m<T> = sm;
	}

	template<typename T>
	T* get_singleton () {
		return m<T>;
	}
}

#endif

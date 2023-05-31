#ifndef GLOBAL_CONFIG_H
#define GLOBAL_CONFIG_H

class GlobalConfig {
	public:
		virtual void init_shaders () = 0;
		virtual void init_screen () = 0;
};

#endif

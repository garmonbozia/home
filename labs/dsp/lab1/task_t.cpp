#ifndef TASK_T_HPP
#define TASK_T_HPP

class task_t
{
public:
	char * file_in;
	char * file_out;
	char * file_model;
	enum mode_t { encoding=0, decoding=1 } mode;
	bool adaptive;
	bool with_memory;
	bool with_model;
};

#endif /* TASK_T_HPP */

#pragma once

enum {
    T_INT, T_FLOAT
};

typedef struct {
    int type;
    int i;
    double f;
} Type;

extern int global_type;

#define compute_op(res, a, op, b) 	\
	if(global_type == T_INT) { 		\
		res.type = T_INT; 			\
		(res).i = (a).i op (b).i; 	\
		(res).f = (double)(res).i; 	\
	} else { 						\
		res.type = T_FLOAT; 		\
		(res).f = (a).f op (b).f; 	\
		(res).i = (int)(res).f; 	\
	}

#define compute_func(res, func, a, b) 	\
	if(global_type == T_INT) { 			\
		res.type = T_INT; 				\
		(res).i = func((a).i, (b).i); 	\
		(res).f = (double)(res).i; 		\
	} else { 							\
		res.type = T_FLOAT; 			\
		(res).f = func((a).f, (b).f); 	\
		(res).i = (int)(res).f; 		\
	}

#ifndef YYSTYPE
#define YYSTYPE Type
#endif

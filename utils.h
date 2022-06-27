#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <time.h>
#include <errno.h> /* errno */
#include <stdlib.h> /* abort */

#define SUCCESS 0
#define FAIL 1
#define FAILURE -1
#define ENQUEUE_FAIL 1
enum {NA = 0};
/* #if __cplusplus < 201104
#define STATIC_ASSERT(EXP,NAME) enum {NAME = 1 / (EXP)}
#else
#define STATIC_ASSERT(EXP,NAME) static_assert(EXP)
#endif  *//* if __cplusplus<201104 */
#if defined(__cplusplus) && __cplusplus > 201104
#define STATIC_ASSERT(EXP,NAME) static_assert(EXP)
#else
#define STATIC_ASSERT(EXP,NAME) enum {NAME = 1 / (EXP)}
#endif /* if __cplusplus<201104 */




#define UNUSED(x) (void)x

#define DED_MEM(x) (x)0xBADC0FFEE0DDF00D
/*  #define LOG(TYPE, M) fpritnf(stderr, TYPE" Time:%s, File: %s line %d"M,\
								NowToString(), __FILE__, __LINE__); */


#define LOG_I(M) LOG("INF", M)
#define LOG_W(M) LOG("WRN", M)
#define LOG_E(M) LOG("ERR", M)

void LOG(char *msg_type_, char *msg_)
{
	printf("%s msg: %s\n", msg_type_, msg_);
}
#define COMMA ,
#ifndef NDEBUG
	#define DEBUG_ONLY(exp) exp
#else
	#define DEBUG_ONLY(exp) 
#endif

#ifndef NDEBUG
	#define DEBUG_ONLY_ARG(exp) COMMA exp
#else
	#define DEBUG_ONLY_ARG(exp) 
#endif


#define CHECK_ALLOC(x, FREE, FREE2, RETURN_VAL) \
do 											 	\
{										 		\
	if (NULL == x)						 		\
	{									 		\
		free(FREE);								\
		free(FREE2);							\
		return RETURN_VAL;				 		\
	}									 		\
}										 		\
while(0)

#define ABORT_IF_BAD(CONDITION, MSG)		\
do 											 		\
{										 			\
	if (CONDITION)					 				\
	{									 			\
		LOG_E(MSG); 								\
		abort();				 					\
	}									 			\
}										 			\
while(0)

#define RETURN_IF_BAD(CONDITION, RETURN_VAL)		\
do 											 		\
{										 			\
	if (CONDITION)					 				\
	{									 			\
		return RETURN_VAL;				 			\
	}									 			\
}										 			\
while(0)

#define RETURN_AND_PERROR_IF_BAD(CONDITION, PERROR_INPUT)		\
do 											 		\
{										 			\
	if (CONDITION)					 				\
	{												\
		perror(PERROR_INPUT);					 	\
		return errno;				 				\
	}									 			\
}	\
while(0)								

#define RETURN_AND_PERROR_IF_NOTGOOD(WANTED_CONDITION, PERROR_INPUT, RETURN_VAL)		\
do 											 		\
{										 			\
	if (!WANTED_CONDITION)					 				\
	{												\
		perror(PERROR_INPUT);					 	\
		return RETURN_VAL;				 				\
	}									 			\
}	\
while(0)									 			

#define ABORT_AND_PERROR_IF_NOTGOOD(WANTED_CONDITION, PERROR_INPUT)		\
do 											 		\
{										 			\
	if (!WANTED_CONDITION)					 				\
	{												\
		perror(PERROR_INPUT);					 	\
		abort();				 				\
	}									 			\
}	\
while(0)			

#define RETURN_IF_NOTGOOD(WANTED_CONDITION,MSG,  RETURN_VAL)		\
do 											 		\
{										 			\
	if (!WANTED_CONDITION)					 				\
	{												\
		LOG_E(MSG);					 	\
		return RETURN_VAL;				 				\
	}									 			\
}	\
while(0)			
						 			
#define ABORT_IF_NOTGOOD(WANTED_CONDITION,MSG)		\
do 											 		\
{										 			\
	if (!WANTED_CONDITION)					 				\
	{												\
		LOG_E(MSG);					 	\
		abort();				 				\
	}									 			\
}	\
while(0)									 			

#define RETURN_VAL_AND_PERROR_IF_BAD(CONDITION, PERROR_INPUT, RETURN_VAL)		\
do 											 		\
{										 			\
	if (CONDITION)					 				\
	{												\
		perror(PERROR_INPUT);					 	\
		return RETURN_VAL;				 				\
	}									 			\
}										 			\
while(0)
#define ABORT_AND_PERROR_IF_BAD(CONDITION, PERROR_INPUT)		\
do 											 		\
{										 			\
	if (CONDITION)					 				\
	{												\
		perror(PERROR_INPUT);					 	\
		abort();				 				\
	}									 			\
}										 			\
while(0)

#define RETURN_IF_FAIL(x, FAILURE_VALUE, RETURN_VAL)\
do 											 		\
{										 			\
	if (FAILURE_VALUE == x)					 		\
	{									 			\
		return RETURN_VAL;				 			\
	}									 			\
}										 			\
while(0)


#endif /*__UTILS_H__*/


#include <assert.h> /* assert */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* strtoll */
#include <limits.h> /* LLONG_MAX */
#include <string.h> /* strlen, memcpy */

#include "redismodule.h"
#include "calc.h" 

#include "utils.h"
/***************************** Help variables *********************************/

/* vresions settings */
enum {MODULE_VER = 1, APIVER = REDISMODULE_APIVER_1};
enum {ERROR_BUF_LEN = 1024};
enum {MAX_LONG_LONG_LEN = 21};

#define CALC_ERROR  LLONG_MAX

typedef long long int(*CalcOperationFunc_ty)(long long int num1_, long long int num2_);
/************************ Functions Declarations ******************************/
/* init functions */
int RedisModule_OnLoad(RedisModuleCtx *ctx_, RedisModuleString **argv_, int argc_);

/* cmd functions */
int CalcAdd_RedisCommand(RedisModuleCtx *ctx_, RedisModuleString **keys_, int argc);
int CalcSub_RedisCommand(RedisModuleCtx *ctx_, RedisModuleString **keys_, int argc);
int CalcMul_RedisCommand(RedisModuleCtx *ctx_, RedisModuleString **keys_, int argc);
int CalcDiv_RedisCommand(RedisModuleCtx *ctx_, RedisModuleString **keys_, int argc);

/* help functions */
/* general calc  wrapper */
int Calc_RedisCommand(RedisModuleCtx *ctx_, 
                      RedisModuleString **keys_, 
                      int argc,
                      CalcOperationFunc_ty op_func_
                      DEBUG_ONLY_ARG(char error_buf[]));

long long int CalcResult(RedisModuleCtx *ctx_,
                         RedisModuleString **in_keys_, 
                         int in_keys_num_, 
                         CalcOperationFunc_ty op_func_
                         DEBUG_ONLY_ARG(char error_buf[]));

/* get key name and returns converted value*/
static long long int GetNumericVal(RedisModuleCtx *ctx_, 
                                   RedisModuleString *in_key_
                                   DEBUG_ONLY_ARG(char error_buf[]));

/* converts value to long long int, updates error_buf if NDEBUG is not defined */
static long long int Key2NumericVal(RedisModuleKey *in_key_ 
                                    DEBUG_ONLY_ARG(char error_buf[]));

int SetOutKeyVal(RedisModuleCtx *ctx_, 
                RedisModuleString *out_key_,
                long long int numeric_result_
                DEBUG_ONLY_ARG(char error_buf[]));
/* op funcs */

static long long int AddLongLong(long long int num1_, long long int num2_);
static long long int SubLongLong(long long int num1_, long long int num2_);
static long long int MulLongLong(long long int num1_, long long int num2_);
static long long int DivLongLong(long long int num1_, long long int num2_);
/*************************** Functions Definitions ****************************/

                            /* init functions */

int RedisModule_OnLoad(RedisModuleCtx *ctx_, RedisModuleString **conf_args_, int argc)
{
    if (REDISMODULE_ERR == RedisModule_Init(ctx_, "CALC", MODULE_VER, APIVER)) 
    {
        return REDISMODULE_ERR;
    }

    if(REDISMODULE_ERR == RedisModule_CreateCommand(ctx_, 
                                                    "calc.add", 
                                                    CalcAdd_RedisCommand, 
                                                    "write", 
                                                    1, -1, 1))
    {
        return REDISMODULE_ERR;
    }
    if(REDISMODULE_ERR == RedisModule_CreateCommand(ctx_, 
                                                    "calc.sub", 
                                                    CalcSub_RedisCommand, 
                                                    "write", 
                                                    1, -1, 1))
    {
        return REDISMODULE_ERR;
    }
    if(REDISMODULE_ERR == RedisModule_CreateCommand(ctx_, 
                                                    "calc.mul", 
                                                    CalcMul_RedisCommand, 
                                                    "write", 
                                                    1, -1, 1))
    {
        return REDISMODULE_ERR;
    }
    if(REDISMODULE_ERR == RedisModule_CreateCommand(ctx_, 
                                                    "calc.div", 
                                                    CalcDiv_RedisCommand, 
                                                    "write", 
                                                    1, -1, 1))
    {
        return REDISMODULE_ERR;
    }
    return REDISMODULE_OK;

    UNUSED(conf_args_);
    UNUSED(argc);


}

                            /* cmd functions */

int CalcAdd_RedisCommand(RedisModuleCtx *ctx_, RedisModuleString **keys_, int argc)
{
    assert(argc >= 3);

    DEBUG_ONLY(char error_buf[ERROR_BUF_LEN];)

    if(REDISMODULE_ERR == Calc_RedisCommand(ctx_,
                                            keys_,
                                            argc,
                                            AddLongLong
                                            DEBUG_ONLY_ARG(error_buf)))
    {
        char error_reply[ERROR_BUF_LEN];

        sprintf(error_reply, "Add operation failed");

        DEBUG_ONLY(sprintf(error_reply + strlen(error_reply), ": %s", error_buf);)

        sprintf(error_reply + strlen(error_reply), "\n");

        RedisModule_ReplyWithError(ctx_, error_reply);

        return REDISMODULE_ERR;
    }

    RedisModule_ReplyWithSimpleString(ctx_, "OK");
    return REDISMODULE_OK;

}

int CalcSub_RedisCommand(RedisModuleCtx *ctx_, RedisModuleString **keys_, int argc)
{
    assert(argc >= 3);

    DEBUG_ONLY(char error_buf[ERROR_BUF_LEN];)

    if(REDISMODULE_ERR == Calc_RedisCommand(ctx_,
                                            keys_,
                                            argc,
                                            SubLongLong
                                            DEBUG_ONLY_ARG(error_buf)))
    {
        char error_reply[ERROR_BUF_LEN];

        sprintf(error_reply, "Sub operation failed");

        DEBUG_ONLY(sprintf(error_reply + strlen(error_reply), ": %s", error_buf);)

        sprintf(error_reply + strlen(error_reply), "\n");

        RedisModule_ReplyWithError(ctx_, error_reply);

        return REDISMODULE_ERR;
    }
    RedisModule_ReplyWithSimpleString(ctx_, "OK");
    return REDISMODULE_OK;

}

int CalcMul_RedisCommand(RedisModuleCtx *ctx_, RedisModuleString **keys_, int argc)
{
    assert(argc >= 3);

    DEBUG_ONLY(char error_buf[ERROR_BUF_LEN];)

    if(REDISMODULE_ERR == Calc_RedisCommand(ctx_,
                                            keys_,
                                            argc,
                                            MulLongLong
                                            DEBUG_ONLY_ARG(error_buf)))
    {
        char error_reply[ERROR_BUF_LEN];

        sprintf(error_reply, "Mul operation failed");

        DEBUG_ONLY(sprintf(error_reply + strlen(error_reply), ": %s", error_buf);)

        sprintf(error_reply + strlen(error_reply), "\n");

        RedisModule_ReplyWithError(ctx_, error_reply);

        return REDISMODULE_ERR;
    }
    RedisModule_ReplyWithSimpleString(ctx_, "OK");
    return REDISMODULE_OK;

}
int CalcDiv_RedisCommand(RedisModuleCtx *ctx_, RedisModuleString **keys_, int argc)
{
    assert(argc >= 3);

    DEBUG_ONLY(char error_buf[ERROR_BUF_LEN];)

    if(REDISMODULE_ERR == Calc_RedisCommand(ctx_,
                                            keys_,
                                            argc,
                                            DivLongLong
                                            DEBUG_ONLY_ARG(error_buf)))
    {
        char error_reply[ERROR_BUF_LEN];

        sprintf(error_reply, "Div operation failed");

        DEBUG_ONLY(sprintf(error_reply + strlen(error_reply), ": %s", error_buf);)

        sprintf(error_reply + strlen(error_reply), "\n");

        RedisModule_ReplyWithError(ctx_, error_reply);

        return REDISMODULE_ERR;
    }
    RedisModule_ReplyWithSimpleString(ctx_, "OK");
    return REDISMODULE_OK;

}
                            /* ccmd wrapper function */

int Calc_RedisCommand(RedisModuleCtx *ctx_, 
                      RedisModuleString **keys_, 
                      int argc,
                      CalcOperationFunc_ty op_func_
                      DEBUG_ONLY_ARG(char error_buf[]))
{   
    

    /* op all in_key */
    long long int numeric_result = CalcResult(ctx_,
                                              keys_ + 1, //skip module name arg
                                              argc - 2, //minus module name + out_key)
                                              op_func_
                                              DEBUG_ONLY_ARG(error_buf));
    
    if(CALC_ERROR == numeric_result)
    {
        return REDISMODULE_ERR;
    }

    /* set out_key val & return status*/

    return SetOutKeyVal(ctx_, 
                        keys_[argc - 1], 
                        numeric_result
                        DEBUG_ONLY_ARG(error_buf));

}
/************************ Help Functions definitions **************************/
int SetOutKeyVal(RedisModuleCtx *ctx_, 
                RedisModuleString *out_key_,
                long long int numeric_result_
                DEBUG_ONLY_ARG(char error_buf[]))
{
    /* convert result to string */
    char result_val_str[MAX_LONG_LONG_LEN];
    sprintf(result_val_str, "%lld", numeric_result_);

    /* open output_key  */
    RedisModuleKey *output_key = RedisModule_OpenKey(ctx_, out_key_, REDISMODULE_WRITE);
    /* set size of output_key value */
    if(REDISMODULE_ERR == RedisModule_StringTruncate(output_key, strlen(result_val_str)))
    {   
        DEBUG_ONLY(sprintf(error_buf, "RedisModule_StringTruncate failed");)
        return REDISMODULE_ERR;
    }

    /* get output_key value  */
    size_t len = 0;
    char *output_key_value = RedisModule_StringDMA(output_key, &len, REDISMODULE_WRITE);
    if(NULL == output_key_value)
    {
        DEBUG_ONLY(sprintf(error_buf, "RedisModule_StringDMA of out_key failed");)
        return REDISMODULE_ERR;

    }

    /* set out_key value to result_str */
    memcpy(output_key_value, result_val_str, strlen(result_val_str));
    RedisModule_CloseKey(output_key);

    return REDISMODULE_OK;

}



long long int CalcResult(RedisModuleCtx *ctx_,
                         RedisModuleString **in_keys_, 
                         int in_keys_num_, 
                         CalcOperationFunc_ty op_func_
                         DEBUG_ONLY_ARG(char error_buf[]))
{
    long long int result = GetNumericVal(ctx_, 
                                         in_keys_[0]
                                         DEBUG_ONLY_ARG(error_buf));

    for(int in_key_idx = 1 ; in_key_idx < in_keys_num_ ; ++in_key_idx)
    {
        
        long long int numeric_val  = GetNumericVal(ctx_, 
                                     in_keys_[in_key_idx]
                                     DEBUG_ONLY_ARG(error_buf));

        
        if(CALC_ERROR == numeric_val)
        {
            return CALC_ERROR;
        }
        /* perform operation */
        result = op_func_(result, numeric_val);

        /* TODO assert overflow */

    }
    
    return result;

}

static long long int GetNumericVal(RedisModuleCtx *ctx_, 
                                   RedisModuleString *in_key_
                                   DEBUG_ONLY_ARG(char error_buf[]))    
{
    /* open key */
        RedisModuleKey *in_key = RedisModule_OpenKey(ctx_, in_key_, REDISMODULE_READ);
        if(NULL == in_key)
        {
            DEBUG_ONLY(sprintf(error_buf, "key doesnt exist");)
            return CALC_ERROR;            
        }

        /* check that is string */
        if(REDISMODULE_KEYTYPE_STRING != RedisModule_KeyType(in_key))
        {
            DEBUG_ONLY(sprintf(error_buf, REDISMODULE_ERRORMSG_WRONGTYPE);)
            return CALC_ERROR;
            
        }
            
            /* convert to long long int */
        long long int numeric_val = Key2NumericVal(in_key DEBUG_ONLY_ARG(error_buf));

        if(CALC_ERROR == numeric_val)
        {
            return CALC_ERROR;
        }

        RedisModule_CloseKey(in_key);
       

        return numeric_val;
}

static long long int Key2NumericVal(RedisModuleKey *in_key_ 
                                    DEBUG_ONLY_ARG(char error_buf[]))
{
    size_t len = 0;
    const char *val = RedisModule_StringDMA(in_key_, &len, REDISMODULE_READ);
    if(NULL == val)
    {
        DEBUG_ONLY(sprintf(error_buf, "RedisModule_StringDMA failed");)
        return CALC_ERROR;

    }
    
    char *endptr;
    long long int ret = strtoll(val, &endptr, 0);

    if(val == endptr || 0 != *endptr)
    {
        DEBUG_ONLY(
            const RedisModuleString *key_redisString_name = RedisModule_GetKeyNameFromModuleKey(in_key_);

            sprintf(error_buf, 
                    "value \"%s\" of key \"%s\" is invalid",
                    val,
                    RedisModule_StringPtrLen(key_redisString_name, &len));)
        return CALC_ERROR;
    }

    return ret;



}

/* op funcs */

static long long int AddLongLong(long long int num1_, long long int num2_)
{
    return num1_ + num2_;
}
static long long int SubLongLong(long long int num1_, long long int num2_)
{
    return num1_ - num2_;
}
static long long int MulLongLong(long long int num1_, long long int num2_)
{
    return num1_ * num2_;
}
static long long int DivLongLong(long long int num1_, long long int num2_)
{
    
    return num1_ / num2_;
}

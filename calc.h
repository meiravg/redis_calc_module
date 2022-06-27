#ifndef __CALC_H__
#define __CALC_H__ 

/****************************
 *  CALCULATOR MODULE V01       *
 *  
 * WRITTEN BY: MEIRAV GRIMBERG *   
 *   
 * LOADING CMD: redis-server --loadmodule your/lib/path/lib$calc.so  
 *                 *
 * NOTES:  1. All commands input keys should contain valid numeric values *
 *             meaning: ! begin with spaces 
 *                      ! followed by a single optional sign (+/-)   *
 *                      ! followed by optional base prefix specifier:
 *                        - "0x" or "0X" is base 16                       *
 *                        - '0' is base 8 (octal)
 *                        - otherwise (no prefix) base 10 (decimal) 
 *                      ! followed by digits [for 16 base] and  A-F lowercase or uppercase
 *                      ! no additional charcters are allowed
 *          2. number of input keys (N) is limited by MAX_INT*
 *          3. out_key value type is long long int, the user is responsible to handle 
 *              possible overflows       
 *          4. key_out max value is  9,223,372,036,854,775,807 - 1*
 *          4. output key should be provided. Its value will be changed *
 *          5. arguments after out_key are ignored *
 * ERRORS: 
 ***************************/ 

/* CALC.ADD <in_key1> <in_key2> ... <in_keyN> <out_key> - adds the values of N keys and store it in the output key */



/*CALC.SUB <in_key1> <in_key2> ... <in_keyN> <out_key> - subtract the values of N keys and store it in the output key
CALC.MUL <in_key1> <in_key2> ... <in_keyN> <out_key> - multiplies the values of N keys and store it in the output key
CALC.DIV <in_key1> <in_key2> ... <in_keyN> <out_key> - divides the values of N keys and store it in the output key
you can use the SET command to set numeric values outside of your module */


#endif /* __CALC_H__ */

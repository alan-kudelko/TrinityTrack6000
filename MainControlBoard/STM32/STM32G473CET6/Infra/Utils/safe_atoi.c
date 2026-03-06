/**
 * @file safe_atoi.c
 * @brief Safe string to integer conversion function.
 * This function attempts to convert a string to an integer value while performing error checking to ensure that
 * the conversion is successful and that the resulting value is within a valid range. It returns true if the conversion
 * is successful and the value is valid, and false otherwise.
 * @param str: Pointer to the input string to be converted.
 * @param length: Length of the input string in bytes.
 * @param result: Pointer to a uint8_t variable where the converted integer value will be stored if the conversion is successful.
 * @return true if the conversion is successful and the value is valid, false otherwise.
 * @note This function is designed to be safe and robust, preventing potential issues such as buffer overflows,
 * invalid input formats, and out-of-range values. It should be used whenever there is a need to convert user input from the CLI interface
 * to integer values for further processing, ensuring that the system can handle invalid input gracefully without crashing
 * or exhibiting undefined behavior. The implementation of this function should include checks for valid numeric characters, proper handling
 * of optional leading whitespace and signs, and validation of the resulting integer value against expected ranges for the specific use case.
 * @note It supports conversion of decimal, hexadecimal (with "0x" prefix) and binary (with "0b" prefix) string formats to integer values, providing
 * flexibility in how users can input numeric values in the CLI interface. The function should also handle edge cases such as empty strings,
 * strings with only whitespace, and strings with invalid characters, returning false in those cases to indicate that the conversion was
 * unsuccessful and the input was invalid.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

bool safe_atoi(const char*str,uint8_t length,uint8_t*result){
    // Temporary value to store the converted integer value, we can use uint32_t to handle larger values
    // and then check if the converted value fits into uint8_t range before assigning it to the result variable
    uint32_t value=0;
    // Empty string or string with only whitespace characters or null pointers
    if((length==0)||(str==NULL)||(result==NULL)){
        return false;
    }
    // String with invalid characters (non-numeric characters, except for optional leading whitespace and signs)
    uint8_t idx=0;

    while((idx<length)&&((str[idx]==' ')||(str[idx]=='\t'))){
        idx++;
    }
    if(idx>=length){
        return false;
    }
    // Base detection logic
    uint32_t base=10;

    if((idx+1)<length){
        if((str[idx]=='0')&&((str[idx+1]=='x')||(str[idx+1]=='X'))){
            base=16;
            idx+=2;
        }
        else{
            if((str[idx]=='0')&&((str[idx+1]=='b')||(str[idx+1]=='B'))){
                base=2;
                idx+=2;
            }
        }
    }
    if(idx>=length){
        return false;
    }
    // Conversion loop
    while(idx<length){
        uint8_t c=str[idx];
        uint32_t digit=0;

        // Character digit mapping based on the detected base
        if((c>='0')&&(c<='9')){
            digit=c-'0';
        }
        else{
            if((c>='a')&&(c<='f')){
                digit=c-'a'+10;
            }
            else{
                if((c>='A')&&(c<='F')){
                    digit=c-'A'+10;
                }
                else{
                    return false;
                }
            }
        }
        // Digit validation based on the detected base
        if(digit>=base){
            return false;
        }
        // Overflow check before multiplying the current value by the base and adding the new digit
        if(value>(UINT8_MAX-digit)/base){
            return false;
        }

        value=value*base+digit;
        idx++;
    }
    // Final range check to ensure the converted value fits into uint8_t
    if(value>UINT8_MAX){
        return false;
    }
    *result=(uint8_t)value;
    return true;
}
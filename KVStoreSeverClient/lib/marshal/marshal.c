/************************************************
 *                     GW KV
 *  https://github.com/gwAdvNet2015/gw-kv-store
 *
 * Copyright 2015 Pradeep Kumar, Yang Hu, Grace Liu
 * This program is licensed under the MIT license.
 *
 * marshal.c - This does all the marshaling
 *************************************************/

/*
 * @desc: The function does the marshing. Function does all the allocation.
 *        It is caller's responsibility to free the memory allocated.
 * @data: Input, the function expecting this structure.
 * @ascii: Function returns this char.
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "marshal.h"
#include "../../common.h"

volatile int verbose_print = 0;

int
gwkv_marshal_server(struct operation* data, int status, char** ascii)
{
        char* val = (char*)malloc(100000*sizeof(char));
        char comm_name[]="set ";
        char flag_exp[]=" 0 0 ";
        char v_len[20];
        char space[] = " ";
        char r_n[]="\r\n";
        char b1[] = "VALUE ";
	char b2[] = " 0 ";
	char b3[] = "\r\n";
	char b4[] = "END\r\n";
        switch(data->method_type) {
            case SET:
                strcat(val,comm_name);
                strcat(val,data->key);
                strcat(val,flag_exp);
        	sprintf(v_len,"%d",(int)data->value_length);
                strcat(val,v_len);
                //strcat(val,space);
                strcat(val,r_n);
                strcat(val,data->value);
                strcat(val,r_n);

                //Convert the status into a string like this:
                //
                break;
            case GET:
                //convert the struct value to like this:
                //VALUE <key> <flags> <bytes> \r\n
                //<data block> \r\n
		//END \r\n
		sprintf(v_len,"%d",(int)data->value_length);
                strcat(val,b1);			//VALUE .
                strcat(val,data->key);		//<key>.
                strcat(val,b2); 		// <flag> .
	    	strcat(val,v_len);		//<bytes>.
		strcat(val,b3);			//\r\n.
		if (data->value != NULL) {
                        strcat(val,data->value);        //<data block>.
                }
		strcat(val,b3);			//\r\n.
		strcat(val,b4);			//END \r\n.
                kvprintf("[!] server marshaling GET");
		break;
       }
       *ascii = val;
       return 0;
}



#define COMMAND_LENGTH 3
#define FLAG_LENGTH 1 //right now 0
#define EXP_TIME_LENGTH 1 //unix time format. Right now 0.
#define SPACE_LENGTH 1
#define NEWLINE_LENGTH 2

int
gwkv_marshal_client(struct operation* data, char** ascii)
{
        char* final_marshed_value = NULL;
        char space = ' ';
        char zero = '0';
        size_t size = 0;
        char value_length[32] = {0};
        char* marshaled_value = 0;

        switch(data->method_type) {
            case SET:
                sprintf(value_length, "%d", data->value_length);

                size = COMMAND_LENGTH + 5*SPACE_LENGTH +
                       FLAG_LENGTH +  EXP_TIME_LENGTH +
                       2* NEWLINE_LENGTH +
                       data->key_length +
                       strlen(value_length) + data->value_length;

                //convert data to this format:
                //<command name> <key> <flags> ...
                marshaled_value = calloc(1, size);
                final_marshed_value = marshaled_value;

                snprintf(marshaled_value, COMMAND_LENGTH + 1,"%s", "set");
                marshaled_value += COMMAND_LENGTH;
                snprintf(marshaled_value, SPACE_LENGTH + 1,"%c", space);
                marshaled_value += SPACE_LENGTH;
                snprintf(marshaled_value, data->key_length +1 ,"%s", data->key);
                marshaled_value += data->key_length;
                snprintf(marshaled_value, SPACE_LENGTH + 1,"%c", space);
                marshaled_value += SPACE_LENGTH;
                snprintf(marshaled_value, FLAG_LENGTH + 1 ,"%c", zero);
                marshaled_value += FLAG_LENGTH;
                snprintf(marshaled_value, SPACE_LENGTH + 1, "%c", space);
                marshaled_value += SPACE_LENGTH;
                snprintf(marshaled_value, EXP_TIME_LENGTH + 1 ,"%c", zero);
                marshaled_value += EXP_TIME_LENGTH;
                snprintf(marshaled_value, SPACE_LENGTH +1 ,"%c", space);
                marshaled_value += SPACE_LENGTH;


                snprintf(marshaled_value, strlen(value_length) + 1 ,"%s", value_length);
                marshaled_value += strlen(value_length);
                //snprintf(marshaled_value, SPACE_LENGTH + 1,"%c", space);
                //marshaled_value += SPACE_LENGTH;
                snprintf(marshaled_value, NEWLINE_LENGTH + 1 ,"%s", "\r\n");
                marshaled_value += NEWLINE_LENGTH;
                //snprintf(marshaled_value, SPACE_LENGTH ,"%s", space);
                //marshaled_value += SPACE_LENGTH;
                snprintf(marshaled_value, data->value_length + 1 ,"%s", data->value);
                marshaled_value += data->value_length;
                snprintf(marshaled_value, NEWLINE_LENGTH ,"%c", '\r');
                marshaled_value += 1;
                snprintf(marshaled_value, NEWLINE_LENGTH ,"%c", '\n');

                break;

            case GET:
                //convert data to this format
                // get <key>* \r\n
                size = COMMAND_LENGTH + SPACE_LENGTH + NEWLINE_LENGTH +
                       data->key_length ;

                marshaled_value = calloc(1, size);
                final_marshed_value = marshaled_value;
                //fwrite("get", sizeof(char), COMMAND_LENGTH, marshaled_value);
                //marshaled_value += COMMAND_LENGTH;
                //fwrite(marshaled_value, SPACE_LENGTH ,"%s", space);
                snprintf(marshaled_value, COMMAND_LENGTH + 1,"%s", "get");
                marshaled_value += COMMAND_LENGTH;
                snprintf(marshaled_value, SPACE_LENGTH + 1,"%c", space);
                marshaled_value += SPACE_LENGTH;
                snprintf(marshaled_value, data->key_length + 1,"%s", data->key);
                marshaled_value += data->key_length;
                snprintf(marshaled_value, NEWLINE_LENGTH ,"%c", '\r');
                marshaled_value += 1;
                snprintf(marshaled_value, NEWLINE_LENGTH ,"%c", '\n');


                break;
            default:
                assert(0);
        }
        *ascii = final_marshed_value;
        return 0;
}

int
gwkv_demarshal_server(char* ascii, struct operation** op)
{
        /* SET: convert ascii to operation
           GET: convert ascii to operation
        }*/
        char* traverse = ascii;
        struct operation *data = (struct operation*)malloc(sizeof(struct operation));
        char s1[] = "set";
        char s2[] = "get";

        if ( 0 == strncmp(ascii, s1, strlen(s1))) {
                kvprintf("[!] server demarshal SET: %s\n", ascii);
                data->method_type = SET;
                traverse += strlen(s1) + 1;

                //next charcater is space;
                assert(traverse[-1] == ' ');


                char* temp = strchr(traverse, ' ');
                if ( NULL != temp) {
                        data->key_length = temp - traverse;
                        data->key = malloc(data->key_length + 1);
                        strncpy(data->key, traverse, data->key_length);
                        //data->key[data->key_length] = '\0';
                } else {
                        assert(0);
                }

                traverse = temp + 1;
                assert(traverse[0] == '0');//flags
                assert(traverse[1] == ' ');
                assert(traverse[2] == '0');//exp time
                assert(traverse[3] == ' ');

                traverse += 4;//pointing to <bytes> i.e. length.

                temp = strchr(traverse, '\r');
                if( NULL != temp) {
                        sscanf(traverse, "%d", &data->value_length);
                } else {
                        assert(0);
                }

                traverse = temp + 1;//pointing to \r\n.

                assert(traverse[0] == '\n');

                traverse += 1; // pointing to data

                temp = strchr(traverse, '\r');
                if ( NULL != temp) {
                        //verify the length
                        assert(data->value_length == temp - traverse);
                        data->value = malloc(data->value_length + 1);
                        strncpy(data->value, traverse, data->value_length);

                } else {
                        assert(0);
                }
                traverse = temp;
                assert(traverse[1] == '\n');



        } else if (0 == strncmp(ascii, s2, strlen(s2))) {
                
                kvprintf("[!] server demarshal GET: %s\n", ascii);
                data->method_type = GET;
                traverse += strlen(s2) + 1;//pointing key
                char* temp = strchr(traverse, '\r');

                //Found the newline
                if (NULL != temp) {
                        data->key_length = temp - traverse;
                        data->key = malloc(data->key_length + 1);
                        strncpy(data->key, traverse, data->key_length);
                } else {
                        assert(0);
                        return -1;
                }
        } else {
                assert(0);
        }

        *op = data;
        return 0;
}

int
gwkv_demarshal_client(char* ascii, struct operation** op, int* status)
{
        struct operation *data = (struct operation*)malloc(sizeof(struct operation));
        *op = data;
        char s0[]="STORED\r\n";
        char s1[]="NOT_STORED\r\n";
        char s2[]="EXISTS\r\n";
        char s3[]="NOT_FOUND\r\n";

        if(strcmp(ascii,s0)==0){
                kvprintf("[!] client demarshal SET: %s\n", ascii);
                data->method_type=SET;
                *status=0;
                return 0;
        }
        else if(strcmp(ascii,s1)==0){
                kvprintf("[!] client demarshal SET: %s\n", ascii);
                data->method_type=SET;
                *status=1;
                return 0;
        }
        else if(strcmp(ascii,s2)==0){
                kvprintf("[!] client demarshal SET: %s\n", ascii);
                data->method_type=SET;
                *status=2;
                return 0;
        }
        else if(strcmp(ascii,s3)==0){
                kvprintf("[!] client demarshal SET: %s\n", ascii);
                data->method_type=SET;
                *status=3;
                return 0;
        }

        kvprintf("[!] client demarshal GET: %s\n", ascii);

        char *value = "VALUE";
        char* traverse = ascii;
        if( 0 != strncmp(traverse, value, strlen(value))) {
                assert(0);
        }

        data->method_type  = GET;
        traverse +=  strlen(value) + 1; // points to key now.

        char* temp = strchr(traverse, ' ');
        if ( NULL != temp) {
                data->key_length = temp - traverse;
                data->key = malloc(data->key_length + 1);
                strncpy(data->key, traverse, data->key_length);
        } else {
                assert(0);
        }

        assert(temp[1] == '0'); //flags
        assert(temp[2] == ' ');
        traverse = temp + 3; //pointing to <bytes> now

        temp = strchr(traverse, '\r');
        if (NULL != temp) {
                sscanf(traverse, "%d", &data->value_length);
		if (data->value_length == 0 ) {
			return 0;
		}
                data->value = malloc(data->value_length);
		
        }

        assert(temp[1] == '\n');

        traverse = temp + 2;//pointing to value now.
        temp = strchr(traverse, '\r');
        if( temp != NULL) {
                //verify the length
                assert(data->value_length == temp - traverse);
                strncpy(data->value, traverse, data->value_length);
        }
        return 0;
}

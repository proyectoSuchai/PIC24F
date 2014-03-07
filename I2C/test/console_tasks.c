/*                            SERIAL CONSOLE
 *                              For PIC24F
 *
 *      Copyright 2012, Francisco Reyes Aspe, komodotas@gmail.com
 *      Copyright 2012, Carlos Gonzalez Cortes, carlgonz@ug.uchile.cl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "console_tasks.h"

char con_buf[CON_BUF_WIDTH];
char con_buf_index;

char con_entry[CON_BUF_WIDTH];
char con_entry_index;
BOOL con_entry_flag; //TRUE if a new attemp of command has arrived

char con_cmd[CON_BUF_WIDTH];
char con_cmd_index;

char con_args[CON_ARG_QTY][CON_ARG_WIDTH];

char con_arg_count;

BOOL con_arg_tolong;

char con_prev_char;

BOOL con_escape_flag;
BOOL con_arrow_flag;

char con_newchar;
BOOL con_init_flag;

/*------------------------------------------------------------------------------
 *		 	CONSOLE COMMAND HANDLER
 *------------------------------------------------------------------------------
 * Description        : Analyze command and chose the correct to be evaluated
 * Arguments          : void
 * Return Value       : void
 *----------------------------------------------------------------------------*/
char buff[100];
void con_cmd_handler(void)
{
    if(con_entry_flag && con_entry_index > 0 && !con_arg_tolong)
    {
       con_cmd_from_entry(con_entry);
       con_printf("\r\n");
       
       if(con_arg_tolong)
       {
           con_printf("Arg too long. Max lenght = ");
           con_putcl(CON_ARG_WIDTH + 48);
           con_printf("\r\n>>");
           con_arg_tolong = FALSE;
           con_entry_flag = FALSE;
           return;
       }

       if((strcmp(con_cmd, "test") == 0) ||
          (strcmp(con_cmd, "test_m1") == 0))
       {
           char test[10] = {4,1,9,1,9,5,6,7,8,9};
           char recv[10] = {0,0,0,0,0,0,0,0,0,0};
           char len = 10;
           char address[] = {I2C_EEPROM_ID, 5};
           int count = 0;
           int i = 0;

           //Test write
           count = i2c1_master_fputs(test, len, address, 2);

           //Print results
           sprintf(buff, "Test write: ");
           con_printf(buff);
           for(i=0; i<len; i++)
           {
               sprintf(buff, "%d, ", test[i]);
               con_printf(buff);
           }
           sprintf(buff, "\n    Wrote %d from %d bytes\n", count, len);
           con_printf(buff);

           //Test read
           count = i2c1_master_fgets(recv, len, address, 2);

           //Print results
           sprintf(buff, "Test read: ");
           con_printf(buff);
           for(i=0; i<len; i++)
           {
               sprintf(buff, "%d, ", recv[i]);
               con_printf(buff);
           }
           sprintf(buff, "\n    Read %d from %d bytes\n", count, len);
           con_printf(buff);

           con_printf("\n>>");
           con_arg_tolong = FALSE;
           con_entry_flag = FALSE;
           return;
       }
       
       if(strcmp(con_cmd, "test_m2") == 0)
       {
           char test[10] = {3,1,2,1,2,5,6,7,8,2};
           char recv[10] = {0,0,0,0,0,0,0,0,0,0};
           char len = 10;
           char address[] = {I2C_EEPROM_ID, 3};
           int count = 0;
           int i = 0;

           //Test write
           count = i2c2_master_fputs(test, len, address, 2);

           //Print results
           sprintf(buff, "Test write: ");
           con_printf(buff);
           for(i=0; i<len; i++)
           {
               sprintf(buff, "%d, ", test[i]);
               con_printf(buff);
           }
           sprintf(buff, "\n    Wrote %d from %d bytes\n", count, len);
           con_printf(buff);

           //Test read
           count = i2c2_master_fgets(recv, len, address, 2);

           //Print results
           sprintf(buff, "Test read: ");
           con_printf(buff);
           for(i=0; i<len; i++)
           {
               sprintf(buff, "%d, ", recv[i]);
               con_printf(buff);
           }
           sprintf(buff, "\n    Read %d from %d bytes\n", count, len);
           con_printf(buff);

           con_printf("\n>>");
           con_arg_tolong = FALSE;
           con_entry_flag = FALSE;
           return;
       }

       if(strcmp(con_cmd, "test_m3") == 0)
       {
           char test[10] = {3,1,8,1,2,8,6,7,8,1};
           char recv[10] = {0,0,0,0,0,0,0,0,0,0};
           char len = 10;
           char address[] = {I2C_EEPROM_ID, 3};
           int count = 0;
           int i = 0;

           //Test write
           count = i2c3_master_fputs(test, len, address, 2);

           //Print results
           sprintf(buff, "Test write: ");
           con_printf(buff);
           for(i=0; i<len; i++)
           {
               sprintf(buff, "%d, ", test[i]);
               con_printf(buff);
           }
           sprintf(buff, "\n    Wrote %d from %d bytes\n", count, len);
           con_printf(buff);

           //Test read
           count = i2c3_master_fgets(recv, len, address, 2);

           //Print results
           sprintf(buff, "Test read: ");
           con_printf(buff);
           for(i=0; i<len; i++)
           {
               sprintf(buff, "%d, ", recv[i]);
               con_printf(buff);
           }
           sprintf(buff, "\n    Read %d from %d bytes\n", count, len);
           con_printf(buff);

           con_printf("\n>>");
           con_arg_tolong = FALSE;
           con_entry_flag = FALSE;
           return;
       }

       if(strcmp(con_cmd, "test_m1s2") == 0)
       {
           char test[10] = {0,1,2,3,4,5,6,7,8,9};
           char recv[10] = {0,0,0,0,0,0,0,0,0,0};
           char len = 10;
           char address[] = {0x0B, 3};
           int count = 0;
           int i = 0;

           //Test write
           count = i2c1_master_fputs(test, len, address, 2);

           //Print results
           sprintf(buff, "Test write: ");
           con_printf(buff);
           for(i=0; i<len; i++)
           {
               sprintf(buff, "%d, ", test[i]);
               con_printf(buff);
           }
           sprintf(buff, "\n    Wrote %d from %d bytes\n", count, len);
           con_printf(buff);

           //Test read
           count = i2c1_master_fgets(recv, len, address, 2);

           //Print results
           sprintf(buff, "Test read: ");
           con_printf(buff);
           for(i=0; i<len; i++)
           {
               sprintf(buff, "%d, ", recv[i]);
               con_printf(buff);
           }
           sprintf(buff, "\n    Read %d from %d bytes\n", count, len);
           con_printf(buff);

           con_printf("\n>>");
           con_arg_tolong = FALSE;
           con_entry_flag = FALSE;
           return;
       }

       if(strcmp(con_cmd, "test_m2s1") == 0)
       {
           char test[10] = {0,1,2,3,4,5,6,7,8,9};
           char recv[10] = {0,0,0,0,0,0,0,0,0,0};
           char len = 10;
           char address[] = {0x0A, 3};
           int count = 0;
           int i = 0;

           //Test write
           count = i2c2_master_fputs(test, len, address, 2);

           //Print results
           sprintf(buff, "Test write: ");
           con_printf(buff);
           for(i=0; i<len; i++)
           {
               sprintf(buff, "%d, ", test[i]);
               con_printf(buff);
           }
           sprintf(buff, "\n    Wrote %d from %d bytes\n", count, len);
           con_printf(buff);

           //Test read
           count = i2c2_master_fgets(recv, len, address, 2);

           //Print results
           sprintf(buff, "Test read: ");
           con_printf(buff);
           for(i=0; i<len; i++)
           {
               sprintf(buff, "%d, ", recv[i]);
               con_printf(buff);
           }
           sprintf(buff, "\n    Read %d from %d bytes\n", count, len);
           con_printf(buff);

           con_printf("\n>>");
           con_arg_tolong = FALSE;
           con_entry_flag = FALSE;
           return;
       }

       //to get here the cmd has to fail al the comparisons
       con_error_unknown_cmd();
       con_printf("\n>>");
    }
    
    con_arg_tolong = FALSE;
    con_entry_flag = FALSE;
    return;
}

/*------------------------------------------------------------------------------
 *		 	CONSOLE ERROR COUNT ARGUMENT
 *------------------------------------------------------------------------------
 * Description        : Display a generic quantity argument error
 * Arguments          : void
 * Return Value       : void
 *----------------------------------------------------------------------------*/
void con_error_count_arg(void){
    con_printf("Arguments quantity dismatch.\r\n");
//    con_printf("Type con_help for more info.\r\n");
    return;
}

/*------------------------------------------------------------------------------
 *		 	CONSOLE ERROR INVALID COMMAND
 *------------------------------------------------------------------------------
 * Description        : Display a generic invalid command error
 * Arguments          : void
 * Return Value       : void
 *----------------------------------------------------------------------------*/
void con_error_invalid_arg(void){
    con_printf("Arguments invalid.\r\n");
//    con_printf("Type con_help for more info about commands.\r\n");
    return;
}

/*------------------------------------------------------------------------------
 *		 	CONSOLE ERROR UNKNOWN COMMAND
 *------------------------------------------------------------------------------
 * Description        : Display a generic unknown command error
 * Arguments          : void
 * Return Value       : void
 *----------------------------------------------------------------------------*/
void con_error_unknown_cmd(void){
    con_printf("Unknown command.\r\n");
//    con_printf("Type con_help for available commands.\r\n");
    return;
}

/*------------------------------------------------------------------------------
 *		 	CONSOLE INITIALIZATION
 *------------------------------------------------------------------------------
 * Description        : Initialize the arrays and peripheral necessary to run
 *                      the console.
 * Arguments          : void
 * Return Value       : void
 *----------------------------------------------------------------------------*/


void con_init(void)
{
    unsigned char i;

    for(i = 0; i < CON_BUF_WIDTH; i++){
        con_buf[i] = '\0';
    }
    con_buf_index = 0;
    con_prev_char = 0;

    con_escape_flag = FALSE;
    con_arrow_flag = FALSE;

    con_init_flag = FALSE;

    return;
}
/*------------------------------------------------------------------------------
 *		 	CONSOLE CHAR HANDLER
 *------------------------------------------------------------------------------
 * Description        : Process any new character entered in the console
 * Arguments          : void
 * Return Value       : void
 *----------------------------------------------------------------------------*/

void con_char_handler(char newchar){

    if((newchar>=0x20 || newchar == 0x0D || newchar == 0x08 || newchar == 0x0A)
            && !con_escape_flag){
        con_buf[(int)con_buf_index] = newchar;
        con_buf_index++;
    }

    switch(newchar){
        case 0x08:      //Backspace
            if(con_buf_index>=2 && !con_escape_flag){
                con_buf_index -= 2; //Erase last char
                con_putc(&newchar);
                con_putc(" ");
                con_putc(&newchar);
            }else{
                con_buf_index = 0;
            }

            break;
        case '\r':      //Carriage Return
            if(!con_entry_flag && con_buf_index > 1 && !con_escape_flag){
                memcpy(con_entry, con_buf, con_buf_index - 1);
                con_entry[con_buf_index - 1] = '\0';//convert in string
                con_entry_index = con_buf_index - 1;
                con_buf_index = 0;
                con_entry_flag = TRUE;
            }
            break;
        case '\n':      //newline
            if(!con_entry_flag && con_buf_index > 1 && !con_escape_flag){

                memcpy(con_entry, con_buf, con_buf_index - 1);
                con_entry[con_buf_index - 1] = '\0';//convert in string
                con_entry_index = con_buf_index - 1;
                con_buf_index = 0;
                con_entry_flag = TRUE;
            }
            break;
        case '\e':
            con_escape_flag = TRUE;
            break;
        default:
            if(con_escape_flag){ //space key pressed
                if(con_arrow_flag){ //arrow
                    if(newchar == 'A'){
//                        con_hist_up();
                        con_escape_flag = FALSE;
                        con_arrow_flag = FALSE;
                    }
                    if(newchar == 'B'){
                        con_escape_flag = FALSE;
                        con_arrow_flag = FALSE;
                    }
                    if(newchar == 'C'){
                        //con_hist_right();NOT IMPLEMENTED
                        con_escape_flag = FALSE;
                        con_arrow_flag = FALSE;
                    }
                    if(newchar == 'D'){
                        //con_hist_down();NOT IMPLEMENTED
                        con_escape_flag = FALSE;
                        con_arrow_flag = FALSE;
                    }
                }
                else{
                    if(newchar == '['){
                        con_arrow_flag = TRUE;
                    }
                }
            }
            else{
                con_putc(&newchar);
            }
            break;
    }

    if(con_buf_index >= CON_BUF_WIDTH){
        con_buf_index = 0;
        con_printf("\r\n Command to long \r\n>>");
    }
    con_prev_char = newchar;
    return;
}
/*------------------------------------------------------------------------------
 *		 	CONSOLE COMMAND FROM ENTRY
 *------------------------------------------------------------------------------
 * Description        : Parse the entry to recognize the command and the args
 * Arguments          : void
 * Return Value       : void
 *----------------------------------------------------------------------------*/

void con_cmd_from_entry(char *entry){
    char * pch1;
    char * pch2;
    pch2 = strchr(entry, ' ');

    con_arg_count = 0;

    if(pch2 != NULL){
        con_strncpy(con_cmd, entry, pch2-entry);

        while(pch2 != NULL){

            pch1 = pch2;

            while(*(pch1+1)==' '){
                pch1++;
            }

            pch2 = strchr(pch1+1,' ');

            if( (pch2 != NULL && pch2-pch1-1>CON_ARG_QTY )|| ( pch2 == NULL && strlen(entry)-(pch1-entry+1)>CON_ARG_QTY) ){
                con_arg_tolong = TRUE;
                return;
            }

            if(pch2 != NULL){
                con_strncpy(con_args[(int)con_arg_count], pch1+1,pch2-pch1-1);
            }
            else{
                strcpy(con_args[(int)con_arg_count],pch1+1);

            }

            con_arg_count++;
        }

    }
    else{
        strcpy(con_cmd,entry);
    }

}

/*------------------------------------------------------------------------------
 *		 			String is Int?
 *------------------------------------------------------------------------------
 * Description        : Check if a string is a representation of an integer
 * Arguments          : str - String to be check
 * Return Value       : BOOL - TRUE or FALSE
 * Example            :
 *----------------------------------------------------------------------------*/

BOOL StrIsInt(char *str){

    char *ch;

    ch = str;
    while(*ch!='\0'){
        if(*ch < 48 || *ch > 57){
            return FALSE;
        }
        ch++;
    }
    return TRUE;
}

/*------------------------------------------------------------------------------
 *                              UART1 INTERRUPT
 *------------------------------------------------------------------------------
 * Description        : Each character is recived by UART2 Interrupt. Console
 *                      does an echo of the character and stores the command
 * Arguments          : None
 * Return Value       : None
 * Example            :
 *----------------------------------------------------------------------------*/
void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt(void)
{
    con_newchar = con_getc();

    con_char_handler(con_newchar);

    if(!con_init_flag)
    {
        con_init_flag = TRUE;
        con_buf_index=0;
    }

    IFS0bits.U1RXIF = 0;
}
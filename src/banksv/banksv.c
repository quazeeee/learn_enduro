/* Processed by ecpg (10.12 (Ubuntu 10.12-0ubuntu0.18.04.1)) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "banksv.pcg"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Enduro/X includes: */
#include <atmi.h>
#include <ubf.h>
#include <bank.fd.h>

#define SUCCEED         0
#define FAIL            -1

/**
 * BALANCE service
 */
void BALANCE (TPSVCINFO *p_svc)
{
        int ret=SUCCEED;        
        
        /* exec sql begin declare section */
         
         
        
#line 22 "banksv.pcg"
 char account [ 28 + 1 ] ;
 
#line 23 "banksv.pcg"
 char date [ 15 + 1 ] ;
/* exec sql end declare section */
#line 24 "banksv.pcg"

        
        char message[100+1];
        BFLDLEN len;

        UBFH *p_ub = (UBFH *)p_svc->data;

        fprintf(stderr, "BALANCE got call\n");

        /* Resize the buffer to have some space in... */
        if (NULL==(p_ub = (UBFH *)tprealloc ((char *)p_ub, 1024)))
        {
                fprintf(stderr, "Failed to realloc the UBF buffer - %s\n",
                        tpstrerror(tperrno));
                ret=FAIL;
                goto out;
        }


        /* Read the account field */
        len = sizeof(account);
        if (SUCCEED!=Bget(p_ub, T_ACCNUM, 0, account, &len))
        {
                fprintf(stderr, "Failed to get T_ACCNUM[0]! -  %s\n",
                        Bstrerror(Berror));
                ret=FAIL;
                goto out;
        }

        /* Read the date field */
        len = sizeof(date);
        if (SUCCEED!=Bget(p_ub, T_DATE, 0, date, &len))
        {
                fprintf(stderr, "Failed to get T_DATE[0]! -  %s\n",
                        Bstrerror(Berror));
                ret=FAIL;
                goto out;
        }

        fprintf(stderr, "Got request for account: [%s] date [%s]\n",
                        account, date);

        { ECPGconnect(__LINE__, 0, "mydb" , "user1" , NULL , "con1", 0); }
#line 66 "banksv.pcg"


        /* exec sql begin declare section */
         
        
#line 69 "banksv.pcg"
 double balance ;
/* exec sql end declare section */
#line 70 "banksv.pcg"


        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "select sum ( balance . balance ) from balance where account_number = $1  and data = $2 ", 
	ECPGt_char,(account),(long)28 + 1,(long)1,(28 + 1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, 
	ECPGt_char,(date),(long)15 + 1,(long)1,(15 + 1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, 
	ECPGt_double,&(balance),(long)1,(long)1,sizeof(double), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 72 "banksv.pcg"


        fprintf(stderr, "Retruning balance %lf\n", balance);
        int n=sprintf (message, "Retruning balance %lf\n", balance);

        len = sizeof(message);
        if (SUCCEED!=Bchg(p_ub, T_MESSAGE, 0, (char *)&message, len))
        {
                fprintf(stderr, "Failed to set T_MESSAGE! -  %s\n",
                        Bstrerror(Berror));
                ret=FAIL;
                goto out;
        }

out:
        tpreturn(  ret==SUCCEED?TPSUCCESS:TPFAIL,
                0L,
                (char *)p_ub,
                0L,
                0L);

        { ECPGdisconnect(__LINE__, "con1");}
#line 93 "banksv.pcg"

}

/**
 * Do initialization
 */
int tpsvrinit(int argc, char **argv)
{
        if (SUCCEED!=tpadvertise("BALANCE", BALANCE))
        {
                fprintf(stderr, "Failed to advertise BALANCE - %s\n",
                        tpstrerror(tperrno));
                return FAIL;
        }

        return SUCCEED;
}

/**
 * Do de-initialization
 */
void tpsvrdone(void)
{
        fprintf(stderr, "tpsvrdone called\n");
}
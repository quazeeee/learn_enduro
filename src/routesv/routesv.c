/* Processed by ecpg (10.12 (Ubuntu 10.12-0ubuntu0.18.04.1)) */
/* These include files are added by the preprocessor */
#include <ecpglib.h>
#include <ecpgerrno.h>
#include <sqlca.h>
/* End of automatic include section */

#line 1 "routesv.pcg"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <atmi.h>
#include <ubf.h>
#include <ndebug.h>
#include <ndrstandard.h>
#include <inicfg.h>
#include <exhash.h>
#include <nerror.h>
#include <cconfig.h>
#include <errno.h>

/* Enduro/X includes: */
#include <atmi.h>
#include <ubf.h>
#include <bank.fd.h>

#define SUCCEED         0
#define FAIL            -1

/**
 * ROUTE service
 */
void ROUTE (TPSVCINFO *p_svc)
{
        int ret=SUCCEED;
        double balance;

        /* exec sql begin declare section */
         
        
#line 33 "routesv.pcg"
 char account [ 28 + 1 ] ;
/* exec sql end declare section */
#line 34 "routesv.pcg"


        char date[15+1];
        char message[100+1];
        BFLDLEN len;

        ndrx_inicfg_t *cfg;
        ndrx_inicfg_section_keyval_t *out = NULL;
        ndrx_inicfg_section_keyval_t *val;

        if (NULL==(cfg=ndrx_inicfg_new()))
        {
            NDRX_LOG(log_error, "TESTERROR: failed to make inicfg: %s", Nstrerror(Nerror));
            EXFAIL_OUT(ret);
        }

        if (EXSUCCEED!=ndrx_inicfg_add(cfg, "bank.ini", NULL))
        {
            NDRX_LOG(log_error, "TESTERROR: failed to make resource: %s", Nstrerror(Nerror));
            EXFAIL_OUT(ret);        
        }

        if (EXSUCCEED!=ndrx_inicfg_get_subsect(cfg, NULL, "bank", &out))
        {
            NDRX_LOG(log_error, "TESTERROR: Failed to resolve [bank]: %s",
                    Nstrerror(Nerror));
            EXFAIL_OUT(ret);    
        }

        if (NULL==(val=ndrx_keyval_hash_get(out, "branchcode")))
        {
            NDRX_LOG(log_error, "TESTERROR: Failed to get branchcode!");
            EXFAIL_OUT(ret);
        }

        UBFH *p_ub = (UBFH *)p_svc->data;

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

        /* Return the value in T_MESSAGE field */

        { ECPGconnect(__LINE__, 0, "mydb" , "user1" , NULL , "con1", 0); }
#line 105 "routesv.pcg"


        /* exec sql begin declare section */
         
        
#line 108 "routesv.pcg"
 int branch ;
/* exec sql end declare section */
#line 109 "routesv.pcg"


        { ECPGdo(__LINE__, 0, 1, NULL, 0, ECPGst_normal, "select branch_code from account where account_number = $1 ", 
	ECPGt_char,(account),(long)28 + 1,(long)1,(28 + 1)*sizeof(char), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EOIT, 
	ECPGt_int,&(branch),(long)1,(long)1,sizeof(int), 
	ECPGt_NO_INDICATOR, NULL , 0L, 0L, 0L, ECPGt_EORT);}
#line 111 "routesv.pcg"


        fprintf(stderr, "Retruning val->val %ld\n", strtol(val->val,NULL,0));
        fprintf(stderr, "Retruning branch %d\n", branch);

        if (strtol(val->val,NULL,0) == branch)
        {
                tpforward("BALANCE", p_svc->data, 0, 0);
        }

        int n=sprintf (message, "Account not supported\n");

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
#line 139 "routesv.pcg"

}

/**
 * Do initialization
 */
int tpsvrinit(int argc, char **argv)
{
        if (SUCCEED!=tpadvertise("ROUTE", ROUTE))
        {
                fprintf(stderr, "Failed to advertise ROUTE - %s\n",
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
# learn_enduro
my attempt to learn how enduro/x works

disadvantages:
1) CCONFSRV not used (not enough time). bank.ini loaded from run folder for now
2) there is no docker for tests
3) not all possible cases handled
4) copy paste

Provide ud files, test results, all configuration files, source code and compiled binaries.

ud files: https://github.com/quazeeee/learn_enduro/tree/master/test

test results: https://github.com/quazeeee/learn_enduro/blob/master/test/result.txt

all configuration files:
  https://github.com/quazeeee/learn_enduro/tree/master/conf
  https://github.com/quazeeee/learn_enduro/blob/master/ubftab/bank.fd

compiled binaries: https://github.com/quazeeee/learn_enduro/tree/master/bin

postgres db:

mydb=# select * from account;
 account_number | currency | branch_code 
----------------+----------+-------------
            100 | EUR      | 5000
            300 | CAD      | 5000
            400 | BYN      | 5500
            200 | USD      | 5600
            101 | USD      | 5000
(5 rows)

mydb=# select * from balance;
 id | account_number |    data    | balance 
----+----------------+------------+---------
  1 |            100 | 1994-12-29 |   11111
  2 |            100 | 1995-12-29 |   21111
  3 |            200 | 2000-12-29 |     100
  4 |            300 | 2000-12-29 |     100
  5 |            400 | 2000-12-29 |     100
  6 |            300 | 2000-12-29 |     200
(6 rows)

mydb=# \d
               List of relations
 Schema |      Name      |   Type   |  Owner   
--------+----------------+----------+----------
 public | account        | table    | postgres
 public | balance        | table    | postgres
 public | balance_id_seq | sequence | postgres
(3 rows)

mydb=# \d account
                         Table "public.account"
     Column     |         Type         | Collation | Nullable | Default 
----------------+----------------------+-----------+----------+---------
 account_number | bigint               |           | not null | 
 currency       | character varying(3) |           | not null | 
 branch_code    | character varying(5) |           | not null | 
Indexes:
    "account_pkey" PRIMARY KEY, btree (account_number)
Referenced by:
    TABLE "balance" CONSTRAINT "balance_account_number_fkey" FOREIGN KEY (account_number) REFERENCES account(account_number) ON DELETE CASCADE

mydb=# \d balance
                                Table "public.balance"
     Column     |  Type  | Collation | Nullable |               Default               
----------------+--------+-----------+----------+-------------------------------------
 id             | bigint |           | not null | nextval('balance_id_seq'::regclass)
 account_number | bigint |           |          | 
 data           | date   |           |          | 
 balance        | bigint |           | not null | 
Indexes:
    "balance_pkey" PRIMARY KEY, btree (id)
Foreign-key constraints:
    "balance_account_number_fkey" FOREIGN KEY (account_number) REFERENCES account(account_number) ON DELETE CASCADE

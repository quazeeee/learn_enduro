#!/bin/sh
echo "Check valid account with balance"
ud < /opt/app1/test/acc_with_balance.ud

echo "Check valid account with different from config branch"
ud < /opt/app1/test/different_branch.ud

echo "Check valid account without balance"
ud < /opt/app1/test/no_balance_for_acc.ud

echo "Check invalid account"
ud < /opt/app1/test/non_existing_acc.ud
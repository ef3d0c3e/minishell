complete -c hledger -s 'f' -l 'file' -d 'Input file'
complete -c hledger -l 'rules-file' -d 'CSV-conversion-rules file'
complete -c hledger -l 'separator' -d 'CSV separator'
complete -c hledger -l 'alias' -d 'Rename account'
complete -c hledger -l 'anon' -d 'Anonymize accounts and payees'
complete -c hledger -l 'pivot' -d 'Use some other field/tag for account names'
complete -c hledger -s 'I' -l 'ignore-assertions' -d 'Ignore any balance assertions'
complete -c hledger -s 'b' -l 'begin' -d 'Include postings/txns on or after this date'
complete -c hledger -s 'e' -l 'end' -d 'Include postings/txns before this date'
complete -c hledger -s 'D' -l 'daily' -d 'Multiperiod/multicolumn report by day'
complete -c hledger -s 'W' -l 'weekly' -d 'Multiperiod/multicolumn report by week'
complete -c hledger -s 'M' -l 'monthly' -d 'Multiperiod/multicolumn report by month'
complete -c hledger -s 'Q' -l 'quarterly' -d 'Multiperiod/multicolumn report by quarter'
complete -c hledger -s 'Y' -l 'yearly' -d 'Multiperiod/multicolumn report by year'
complete -c hledger -s 'p' -l 'period' -d 'Set start date, end date, and report interval'
complete -c hledger -l 'date2' -d 'Match secondary date instead'
complete -c hledger -s 'U' -l 'unmarked' -d 'Include only unmarked postings/txns'
complete -c hledger -s 'P' -l 'pending' -d 'Include only pending (“!”) postings/txns'
complete -c hledger -s 'C' -l 'cleared' -d 'Include only cleared (“*”) postings/txns'
complete -c hledger -s 'R' -l 'real' -d 'Include only non-virtual postings'
complete -c hledger -l 'depth' -d 'Hide accounts/postings deeper than this'
complete -c hledger -s 'E' -l 'empty' -d 'Show items with zero amount'
complete -c hledger -s 'B' -l 'cost' -d 'Converts amounts to their cost at txn time'
complete -c hledger -s 'V' -l 'value' -d 'Converts amounts to their market value on report end date'
complete -c hledger -l 'auto' -d 'Apply automated posting rules to modify txns'
complete -c hledger -l 'forecast' -d 'Apply periodic txn rules to generate future txns'
complete -c hledger -s 'h' -l 'help' -d 'Show help for this command'
complete -c hledger -l 'debug' -d 'Show debug output'
complete -c hledger -l 'version' -d 'Show version information'
complete -c hledger -d 'Add transactions using guided prompts'
complete -c hledger -l 'no-new-accounts' -d 'Don’t allow creating new accounts'
complete -c hledger -d 'Add new transactions from other files'
complete -c hledger -l 'dry-run' -d 'Just show the transactions to be imported'
complete -c hledger -d 'Ensure transactions are ordered by date'
complete -c hledger -l 'strict' -d 'Makes date comparing strict'
complete -c hledger -d 'Ensure accounts have different leaf names'
complete -c hledger -d 'Generate balance-resetting transactions'
complete -c hledger -l 'opening' -d 'Show just opening transaction'
complete -c hledger -l 'closing' -d 'Show just closing transaction'
complete -c hledger -d 'Generate automated postings/diffs'
complete -c hledger -l 'add-posting' -d 'Add a posting to account'
complete -c hledger -l 'diff' -d 'Generate diff suitable for patch(1)'
complete -c hledger -d 'Show assets, liabilities, and net worth'
complete -c hledger -d 'Show assets, liabilities, and equity'
complete -c hledger -d 'Show changes in liquid assets'
complete -c hledger -d 'Show revenues and expenses'
complete -c hledger -l 'change' -d 'Show balance change in each period'
complete -c hledger -l 'cumulative' -d 'Show balance change accumulated across periods'
complete -c hledger -s 'H' -l 'historical' -d 'Show historical ending balance in each period'
complete -c hledger -l 'flat' -d 'Show accounts as a list'
complete -c hledger -l 'drop' -d 'Omit N leading account-name parts (in flat mode)'
complete -c hledger -s 'N' -l 'no-total' -d 'Omit the final total row'
complete -c hledger -l 'tree' -d 'Show accounts as a tree; amounts include subaccounts'
complete -c hledger -s 'A' -l 'average' -d 'Show a row average column in multicolumn reports'
complete -c hledger -s 'T' -l 'row-total' -d 'Show a row total column in multicolumn reports'
complete -c hledger -l 'no-elide' -d 'Don’t squash boring parent accounts in tree mode'
complete -c hledger -l 'format' -d 'Use a custom line format in multicolumn reports'
complete -c hledger -l 'pretty-tables' -d 'Use Unicode when displaying tables'
complete -c hledger -s 'S' -l 'sort-amount' -d 'Sort by amount instead of account code/name'
complete -c hledger -s 'O' -l 'output-format' -d 'Select an output format'
complete -c hledger -s 'o' -l 'output-file' -d 'Write output to given file; extension selects format'
complete -c hledger -d 'Show return on investments'
complete -c hledger -l 'cashflow' -d 'Show all amounts that were used to compute returns'
complete -c hledger -l 'investment' -d 'Query to select investment transactions'
complete -c hledger -l 'profit-loss' -l 'pnl' -d 'Query to select profit-and-loss or appreciation/valuation transactions'
complete -c hledger -d 'Show account names'
complete -c hledger -l 'declared' -d 'Show account names declared with account directives'
complete -c hledger -l 'used' -d 'Show account names referenced by transactions'
complete -c hledger -l 'tree' -d 'Show short account names as a tree'
complete -c hledger -l 'flat' -d 'Show short account names as a list'
complete -c hledger -l 'drop' -d 'Omit N leading account name parts in flat mode'
complete -c hledger -d 'Show postings-per-interval bar charts'
complete -c hledger -d 'Show balance changes/end balances/budgets in accounts'
complete -c hledger -l 'change' -d 'Show balance change in each period'
complete -c hledger -l 'cumulative' -d 'Show balance change accumulated across periods'
complete -c hledger -s 'H' -l 'historical' -d 'Show historical ending balance in each period'
complete -c hledger -l 'tree' -d 'Show accounts as a tree'
complete -c hledger -l 'flat' -d 'Show accounts as a list'
complete -c hledger -s 'A' -l 'average' -d 'Show a row average column in multicolumn reports'
complete -c hledger -s 'T' -l 'row-total' -d 'Show a row total column in multicolumn reports'
complete -c hledger -s 'N' -l 'no-total' -d 'Omit the final row'
complete -c hledger -l 'drop' -d 'Omit N leading account name parts in flat mode'
complete -c hledger -l 'no-elide' -d 'Don’t squash boring parent accounts in tree mode'
complete -c hledger -l 'format' -d 'Use a custom line format in multicolumn reports'
complete -c hledger -l 'pretty-tables' -d 'Use Unicode when displaying tables'
complete -c hledger -s 'S' -l 'sort-amount' -d 'Sort by amount instead of account code/name'
complete -c hledger -l 'budget' -d 'Show performance compared to budget goals defined by periodic transactions'
complete -c hledger -l 'invert' -d 'Display all amounts with reversed sign'
complete -c hledger -l 'transpose' -d 'Transpose rows and columns'
complete -c hledger -s 'O' -l 'output-format' -d 'Select an output format'
complete -c hledger -s 'o' -l 'output-file' -d 'Write output to given file; extension selects format'
complete -c hledger -d 'Show commodity/currency symbols'
complete -c hledger -d 'Show input file paths'
complete -c hledger -d 'Show market-price records'
complete -c hledger -l 'costs' -d 'Print transaction prices from postings'
complete -c hledger -l 'inverted-costs' -d 'Print transaction inverted prices from postings also'
complete -c hledger -d 'Show transactions'
complete -c hledger -d 'Show only transactions with unique descriptions'
complete -c hledger -s 'm' -l 'match' -d 'Show the most-recent transaction most similar to STR'
complete -c hledger -s 'x' -l 'explicit' -d 'Show all amounts explicitly'
complete -c hledger -l 'new' -d 'Show only newer-dated transactions added in each file since last run'
complete -c hledger -s 'O' -l 'output-format' -d 'Select an output format'
complete -c hledger -s 'o' -l 'output-file' -d 'Write output to given file; extension selects format'
complete -c hledger -d 'Show postings in one or more accounts & running total'
complete -c hledger -l 'cumulative' -d 'Show running total from report start date'
complete -c hledger -s 'H' -l 'historical' -d 'Show historical running total/balance'
complete -c hledger -s 'A' -l 'average' -d 'Show running average of posting amounts instead of total (implies --empty)'
complete -c hledger -s 'r' -l 'related' -d 'Show postings’ siblings instead'
complete -c hledger -l 'invert' -d 'Display all amounts with reversed sign'
complete -c hledger -s 'w' -l 'width' -d 'Set output width (defaults to terminal width or $COLUMNS)'
complete -c hledger -s 'O' -l 'output-format' -d 'Select an output format'
complete -c hledger -s 'o' -l 'output-file' -d 'Write output to given file; extension selects format'
complete -c hledger -d 'Show a recent posting that best matches a description'
complete -c hledger -d 'Show journal statistics'
complete -c hledger -s 'o' -l 'output-file' -d 'Write output to given file; extension selects format'
complete -c hledger -d 'Show tag names'
complete -c hledger -d 'Run self-tests'
complete -c hledger -d 'Show hledger manual list'

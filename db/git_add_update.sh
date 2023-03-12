# update script that dumps the current db and adds the changes to git cache ready for commit

`mysqldump --no-create-db --order-by-primary --tab=. -uroot -p music && git diff --numstat | grep -ve '^\t\t.*sql$' | grep -ve "^1\t1\t.*\.sql$" | awk '{print $3}' | tr '\n' ' ' | xargs git add`

git status

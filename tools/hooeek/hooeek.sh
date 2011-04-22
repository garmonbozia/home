#!/bin/bash

#############################################################
# usage:    hooeek username [-option] [[option parametres]]
# examples: `hooeek pooq`          some stats    
#           `hooeek pooq -m`       messages number
#           `hooeek pooq -p`       pages number
#           `hooeek pooq -n`       first post #number
#           `hooeek pooq -d`       first post date
#           `hooeek pooq -t`       top user's posts
#           `hooeek pooq -t 20 30` top user's posts from
#                                     20's to 30's pages
#           `hooeek pooq -c`       count comments in all
#                                     user's posts
#
#############################################################

get_pages_number()
{
    local post_per_page=20
    local pages=$(($1 / $post_per_page))
    [[ "$(($1 % $post_per_page))" != "0" ]] && let pages++
    echo "$pages"
    return $pages
}

username=$1
main_page="`curl -s http://juick.com/${username}/`"
messages=`grep -P -o '(?<=(Messages:\ ))([0-9]+)' <<< $main_page`
pages=`get_pages_number $messages`
last_page=`curl -s "http://juick.com/${username}/?show=blog&page=$pages"`
first_post_number=`echo "$last_page" |\
                   grep -P "${username}/([0-9]{1,8})\"([^&])#\1" -o |\
                   sed -r -e 's/.*#(.*)/\1/' |\
                   tail -n 1`
first_post=`curl -s "http://juick.com/${username}/${first_post_number}"`
first_post=`echo $first_post | sed -r -e 's/All.*Messages.*//'`
first_post_date_all=`grep -P -o '\<span\ title(.*?)\<\/span' <<< $first_post`
first_post_date_nearly=`echo "$first_post_date_all" |\
                        sed -r -e 's/.*>(.*)<.*/\1/'`
first_post_date_exactly=`echo "$first_post_date_all" |\
                         sed -r -e 's/.*"(.*GMT).*/\1/'`

if [[ "$#" -eq 1 ]]; then
    echo "messages= $messages"
    echo "pages= $pages"
    echo "first_post_number= $first_post_number"
    echo "first_post_date_nearly= $first_post_date_nearly"
    echo "first_post_date_exactly= $first_post_date_exactly" 
    exit 
fi  

case "$2" in
    -m) #messages number
        echo "$messages"
        exit
    ;;
    -p) #pages number
        echo"$pages"
        exit
    ;;
    -n) #first post number
        echo "$first_post_number"
        exit
    ;;
    -d) #first post date
        echo "$first_post_date_exactly ($first_post_date_nearly)" 
        exit
    ;;
esac

# if -t (search top posts)
if [[ "$2" == "-t" ]]; then
    echo "start searching top @${username} posts"
    [[ "$3" == "" ]] && pages_first=1     || pages_first=$3
    [[ "$4" == "" ]] && pages_last=$pages || pages_last=$4
    for page in `seq $pages_first $pages_last`; do
        echo "* page $page/$pages_last" 
        nth_page=`curl -s "http://juick.com/${username}/?show=blog&page=$page"`
        posts=`echo "$nth_page" |\
               grep -P "${username}/([0-9]{1,8})\"([^&])#\1" -o |\
               sed -r -e 's/.*#(.*)/\1/'`
        for post in $posts; do
            post_text=`curl -s "http://juick.com/${username}/$post"`
            [[ "`grep -P '<a href="/top/">@top</a>' -o <<< $post_text`" != "" ]] \
            && echo "#$post"
        done
    done
    echo "done"
    exit
fi

# if -c (count totally replies number)
if [[ "$2" == "-c" ]]; then
    [[ "$3" == "" ]] && pages_first=1     || pages_first=$3
    [[ "$4" == "" ]] && pages_last=$pages || pages_last=$4

    page=$pages_last
    while [[ $page -gt $(($pages_first-1)) ]]; do
        wget -q -O- "http://juick.com/${username}/?show=blog&page=${page}"
        let page--
    done | grep -o "[0-9]* replies" | awk '{i+=$1}END{print i}'
            # ^ weird ^ wooga ^ shit ^
    exit
fi

